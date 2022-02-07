#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int ch = 0;
    printf("Press number (1, 2, 3, 4): ");
    scanf("%d", &ch);

    switch (ch)
    {
        case 1:
        {
            char command[127];
            printf("Press command: ");
            scanf("%s", command);
            system(command);
            break;
        }
        case 2:
        {
            system("./script.sh");
            break;
        }
        case 3:
        {
            fork();
            if(fork() == 0)
            {
                system("ps -u $USER");
            }

            break;
        }
        case 4:
        {
            puts("exit");
            break;
        }
        default:
        {
            puts("Incorrect case, exit");
        }
    }

    return 0;
}