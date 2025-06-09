#include <stdio.h>		/*printf*/
#include <stdlib.h>
#include <string.h>		/*strcspn*/
#include <netdb.h>      /*gethostbyname*/
#include <arpa/inet.h>  /*inet_ntoa()*/

#define MAX_WORDS 30000
#define MAX_LINE_LENGTH 512

void dns_enumerator(const char *url, const char *words_file)
{
	char fqdn[MAX_LINE_LENGTH];  /*fqdn stands for Fully Qualified Domain Name, like www.google.com*/
	char line[MAX_LINE_LENGTH];  /*buffer to store each line*/
	struct hostent *host;        /*Pointer to host information*/
	char *convrt_ip;
	int i;

	FILE *file = fopen(words_file, "r");
	if(!file)
	{
		perror("fopen");
		return;
	}

	while (fgets(line, sizeof(line), file))
	{
		line[strcspn(line, "\n")] = '\0';  /*Removes the newline character and replaces it with '\0'*/

		/*snprintf is a function that formats and writes a string into a buffer.
		  Here it constructs the full domain name (FQDN) by appending the subdomain (from the wordlist)
		  to the base URL. This is stored in the `fqdn` buffer.*/
		snprintf(fqdn, sizeof(fqdn), "%s.%s", line, url);  

		/*Returns a pointer to a struct hostnet which contains information about the host (like its name and associated ip addresses).
		  It's used to resolve fqdn into an ip address. Returns null if the subdomain doesn't exist*/
		host = gethostbyname(fqdn);  

		i = 0;
		while (host && (host->h_addr_list[i] != NULL))  /*h_addr_list is an array of pointers to the host's ip addresses in binary*/
		{
			/*inet_ntoa converts the binary IP address to a human readable string, so we first have to cast it to a pointer 
			  to the in_addr struct which the inet_ntoa function expects, and then dereference it.*/
			convrt_ip = inet_ntoa(*(struct in_addr*)host->h_addr_list[i]);  
			printf("Subdomain: %s -> IP: %s\n", fqdn, convrt_ip); 
			++i;
		}
	}

	fclose(file);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <URL> <words file>\n", argv[0]);
		return -1;
	}

	dns_enumerator(argv[1], argv[2]);

	return 0;
}