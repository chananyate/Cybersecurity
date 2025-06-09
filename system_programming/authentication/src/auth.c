/*auth.c
  This file implements an authenticator API.
  It stores users' usernames and hashed passwords in a file for future authentication.
  It provides adding, hashing, deleting and authentication functionalities.
  *Author: Chananya Templeman
  *Reviewed by:
  *Date: 30/10/2024*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auth.h"

#define MAX_LINE 256
#define MAX_USERS 100
#define USERNAME_LEN 32
#define PASSWORD_LEN 32
#define DB_FILE "users.db"

struct User 
{
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
};

static struct User users[MAX_USERS];
static int user_count = 0;

/*Simple encryption*/
static void encrypt_password(const char *input, char *output)
{
    int i;
    char key = 42; /*Simple XOR key*/
    
    for(i = 0; input[i] != '\0'; i++) 
        output[i] = input[i] ^ key;
    
    output[i] = '\0';
}

/*Loads users from file*/
void load_users(void)
{
    FILE *file;
    char line[MAX_LINE];
    char *token;
    
    user_count = 0;
    file = fopen(DB_FILE, "r");
    
    if (NULL != file) 
    {
        while (NULL != fgets(line, MAX_LINE, file) && user_count < MAX_USERS) 
        {
            /*Remove the newline character from the string read by fgets. 
              strcspn(line, "\n") returns the index of the first occurrence of the newline character in 'line'. 
              By setting that position to the null terminator '\0', we effectively truncate the string at that point, 
              ensuring it does not contain the newline character.*/
            line[strcspn(line, "\n")] = '\0';
            
            /*Gets username*/
            token = strtok(line, ":");
            if (NULL == token) 
                continue;

            /*Copies the username from file into the username element in the users array*/
            strncpy(users[user_count].username, token, USERNAME_LEN - 1);
            users[user_count].username[USERNAME_LEN - 1] = '\0';
            
            /*Gets password*/
            token = strtok(NULL, ":");
            if (NULL == token) 
                continue;

            strncpy(users[user_count].password, token, PASSWORD_LEN - 1);
            users[user_count].password[PASSWORD_LEN - 1] = '\0';
            
            user_count++;
        }
        fclose(file);
    }
}

/*Saves users to file*/
void save_users(void)
{
    FILE *file;
    int i;
    
    file = fopen(DB_FILE, "w");
    if (NULL != file) 
    {
        for (i = 0; i < user_count; i++) 
            fprintf(file, "%s:%s\n", users[i].username, users[i].password);
    
        fclose(file);
    }
}

int add_user(const char* username, const char* password)
{
    int i;
    char encrypted[PASSWORD_LEN];
    
    /*Checks if username already exists*/
    for(i = 0; i < user_count; i++) 
    {
        if(strcmp(users[i].username, username) == 0) 
            return 0; /*User already exists*/
    }
    
    /*Checks if we have space*/
    if(user_count >= MAX_USERS) 
        return 0; /*No space left*/
    
    /*Adds new user*/
    strncpy(users[user_count].username, username, USERNAME_LEN - 1);
    users[user_count].username[USERNAME_LEN - 1] = '\0';
    
    /*Encrypts and store password*/
    encrypt_password(password, encrypted);
    strncpy(users[user_count].password, encrypted, PASSWORD_LEN - 1);
    users[user_count].password[PASSWORD_LEN - 1] = '\0';
    
    user_count++;
    
    /*Saves changes to file*/
    save_users();
    return 1;
}

int delete_user(const char* username)
{
    int i;
    int found = 0;
    
    for(i = 0; i < user_count; i++) 
    {
        if(strcmp(users[i].username, username) == 0) 
        {
            found = 1;
            break;
        }
    }
    
    if(found) 
    {
        /*Moves all subsequent users up one position*/
        for(; i < user_count - 1; i++) 
        {
            strcpy(users[i].username, users[i + 1].username);
            strcpy(users[i].password, users[i + 1].password);
        }
        user_count--;
        
        /*Saves changes to file*/
        save_users();
        return 1;
    }
    
    return 0;
}

int authenticate(const char* username, const char* password)
{
    int i;
    char encrypted[PASSWORD_LEN];
    
    encrypt_password(password, encrypted);
    
    for(i = 0; i < user_count; i++) 
    {
        if(0 == strcmp(users[i].username, username)) 
            return strcmp(users[i].password, encrypted) == 0;
    }
    
    return 0;
}