/*pam_auth.c
* This file implements a PAM-based user authentication system for "myapp".
* The authentication relies on the Pluggable Authentication Module (PAM) API
* to provide secure and flexible authentication methods.*/

#define _POSIX_C_SOURCE 200809L  /*strdup*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include "pam_auth.h"

/*This function andles communication between PAM and the application. 
   It processes prompts and messages from PAM, including username and password 
   requests, and provides appropriate responses. Allocates and fills a response 
   array based on each message type, storing user responses (like username or 
   password) in dynamically allocated memory. Assigns this array to *resp 
   (a pointer to an array of struct pam_response structures), 
   enabling PAM to access the responses for authentication. PAM will free this 
   response array after completing the conversation, so no cleanup is needed 
   in conv_func.*/
static int conv_func(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
    int i;
    struct pam_response *response;
    char *username;
    char buffer[MAX_INPUT];
    
    /*Allocates memory for responses.
      Allocates memory for an array of pam_response structs.
      Each struct holds a response (like password) for a specific PAM prompt.*/
    response = malloc(num_msg * sizeof(struct pam_response));
    if (response == NULL) 
        return PAM_CONV_ERR;
    
    /*Handles each message.
      Loops through each message sent by pam and responds based on its type.
      The message types include prompts for username, password and informational messages.*/
    for (i = 0; i < num_msg; ++i) 
    {
        switch (msg[i]->msg_style) 
        {
            case PAM_PROMPT_ECHO_ON:
                /*Gets username. Handles prompts where user input is displayed (like username).
                  The username is passed in appdata_ptr and assigned to the response.*/
                username = (char *)appdata_ptr;
                response[i].resp = strdup(username);
                response[i].resp_retcode = 0;
                break;
                
            case PAM_PROMPT_ECHO_OFF:
                /*Gets password*/
                printf("%s", msg[i]->msg);
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                
                response[i].resp = strdup(buffer);
                response[i].resp_retcode = 0;
                break;
                
            case PAM_ERROR_MSG:
                fprintf(stderr, "%s\n", msg[i]->msg);
                response[i].resp = NULL;
                response[i].resp_retcode = 0;
                break;
                
            case PAM_TEXT_INFO:
                /*Displays informational messages*/
                printf("%s\n", msg[i]->msg);
                response[i].resp = NULL;
                response[i].resp_retcode = 0;
                break;
                
            default:
                free(response);
                return PAM_CONV_ERR;
        }
    }
    
    *resp = response;  /*Assigns responses array to resp for PAM*/
    return PAM_SUCCESS;
}

/*Function to authenticate user using PAM.
  This function sets up and initiates a PAM session to authenticate a user.
  It manages the full PAM process, including a setup, authentication, account validation and cleanup.*/
int authenticate_user(const char *username) 
{
    pam_handle_t *pamh;
    struct pam_conv conv;
    int retval;
    
    /*Sets up the PAM conversation.
      Initializes the conversation structure with the conv_func function 
      and passes the username via appdata_ptr*/
    conv.conv = conv_func;
    conv.appdata_ptr = (void*)username;
    
    /*Starts a PAM for the myapp service. Initializes PAM with the conversation function and prepares it for authentication.*/
    retval = pam_start("myapp", username, &conv, &pamh);
    if (retval != PAM_SUCCESS) 
    {
        fprintf(stderr, "Failed to start PAM: %s\n", pam_strerror(pamh, retval));
        return 0;
    }
    
    /*Authenticates*/
    retval = pam_authenticate(pamh, 0);
    if (retval != PAM_SUCCESS) 
    {
        fprintf(stderr, "Authentication failed: %s\n", pam_strerror(pamh, retval));
        pam_end(pamh, retval);
        return 0;
    }
    
    /*Checks if account is valid (not expired or disabled)*/
    retval = pam_acct_mgmt(pamh, 0);
    if (retval != PAM_SUCCESS) 
    {
        fprintf(stderr, "Account validation failed: %s\n", pam_strerror(pamh, retval));
        pam_end(pamh, retval);
        return 0;
    }
    
    /*Ends pam session, freeing any resources associated with pamh*/
    pam_end(pamh, PAM_SUCCESS);
    return 1;
}
