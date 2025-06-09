/*pam_auth.h*/
#ifndef PAM_AUTH_H
#define PAM_AUTH_H

#include <security/pam_appl.h>

#define MAX_INPUT 64
#define PAM_SUCCESS_MSG "Authentication successful!\n"
#define PAM_FAILED_MSG "Authentication failed!\n"

/* Function prototypes */
int authenticate_user(const char *username);
static int conv_func(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr);

#endif /* PAM_AUTH_H */