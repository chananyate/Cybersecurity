/*auth.h*/
#ifndef AUTH_H
#define AUTH_H

/*Function declarations*/
int add_user(const char* username, const char* password);
int delete_user(const char* username);
int authenticate(const char* username, const char* password);
void load_users(void);
void save_users(void);

#endif /*AUTH_H*/