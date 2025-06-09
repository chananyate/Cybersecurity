#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main()
{
	void *handle;
	void (*Foo)(int);
	char *error;

	handle = dlopen("./libfoo.so", RTLD_LAZY);/*Opens the dynamic library*/

	if (!handle)
	{
		fprintf(stderr, "%s\n", dlerror());

		return 1;
	}

	dlerror();/*Clears any previous errors that might be stored because dlopen and dlsym do not clear them automatically.*/

	*(void **)(&Foo) = dlsym(handle, "Foo");/*Gets the address of the function Foo.*/

	if ((error = dlerror()) != NULL)
	{
		fprintf(stderr, "%s\n", error);

		dlclose(handle);

		return 1;
	}

	Foo(10);

	dlclose(handle);/*Close the library*/

	return 0;
}