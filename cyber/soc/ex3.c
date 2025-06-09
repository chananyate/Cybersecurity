#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>

/* Defines a conversation structure for PAM authentication */
static struct pam_conv conv = {
    misc_conv,  /* This is the conversation function to handle user prompts (like password input) */
    NULL        /* This is the optional data associated with the conversation (set to NULL in this case) */
};

int main() {
    pam_handle_t *pamh = NULL;
    int retval;

    /*  Initializes the PAM library and begins the authentication process */
    retval = pam_start("login", NULL, &conv, &pamh);
    if (retval != PAM_SUCCESS) {
        printf("Error: pam_start failed\n");
        return 1;
    }

    /* This triggers the actual authentication and logging (through the configured PAM modules */
    retval = pam_authenticate(pamh, 0);  
    if (retval == PAM_SUCCESS) {
        printf("Authentication successful\n");
    } else {
        printf("Authentication failed\n");
    }

    pam_end(pamh, retval);
    return 0;
}
