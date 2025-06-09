/*main.c*/
#include <stdio.h>
#include <string.h>
#include "auth.h"

#define INPUT_SIZE 32

int main(void)
{
    char username[INPUT_SIZE];
    char password[INPUT_SIZE];
    int choice;
    int result;
    
    /*Loads existing users*/
    load_users();
    
    /*Main program loop*/
    while(1) 
    {  
        printf("\nAuthentication System\n");
        printf("1. Add User\n");
        printf("2. Delete User\n");
        printf("3. Authenticate\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        /*Clears input buffer by advancing the cursor ("pointer") through the input buffer, 
          discarding all characters until it finds the newline, ensuring that any leftover input 
          does not affect subsequent input operations.*/
        while(getchar() != '\n');
        
        switch(choice) 
        {
            case 1:
                /*Prompts the user for a username and reads input from stdin (keyboard) using fgets
                  The input is stored in the 'username' array, and we remove any trailing newline character 
                  using strcspn to remove the newline character from input.*/
                printf("Enter username: ");
                fgets(username, INPUT_SIZE, stdin);
                username[strcspn(username, "\n")] = 0;
                
                printf("Enter password: ");
                fgets(password, INPUT_SIZE, stdin);
                password[strcspn(password, "\n")] = 0;
                
                result = add_user(username, password);
                if(result) 
                    printf("User added successfully!\n");

                else 
                    printf("Failed to add user!\n");
                
                break;
                
            case 2:
                printf("Enter username to delete: ");
                fgets(username, INPUT_SIZE, stdin);
                username[strcspn(username, "\n")] = 0;
                
                result = delete_user(username);
                if(result) 
                    printf("User deleted successfully!\n");

                else 
                    printf("Failed to delete user!\n");
                
                break;
                
            case 3:
                printf("Enter username: ");
                fgets(username, INPUT_SIZE, stdin);
                username[strcspn(username, "\n")] = 0;
                
                printf("Enter password: ");
                fgets(password, INPUT_SIZE, stdin);
                password[strcspn(password, "\n")] = 0;
                
                result = authenticate(username, password);
                if(result) 
                    printf("Authentication successful!\n");

                else 
                    printf("Authentication failed!\n");
                
                break;
                
            case 4:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
    
    return 0;
}