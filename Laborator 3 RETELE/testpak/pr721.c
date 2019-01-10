#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
int FILE = open(const char "./testpak", "-r");
ssize_t source1;
ssize_t source2;
ssize_t source3;
ssize_t source4;

read(FILE, &source1,2);
printf(%d, "source1");

read(FILE, &source2,2);
printf(%d, "source2");

read(FILE, &source3,2);
printf(%d, "source3");

read(FILE, &source4,2);
printf(%d, "source4");

close(*FILE);
}
