/*pam_auth_test.c*/
#include <stdio.h>
#include <string.h>
#include "pam_auth.h"

int main(void)
{
    char username[MAX_INPUT];
    
    /*Gets username*/
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
    
    if (authenticate_user(username)) 
    {
        printf(PAM_SUCCESS_MSG);
        return 0;
    } 
    else 
    {
        printf(PAM_FAILED_MSG);
        return 1;
    }
}