#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pwd.h>
#include <ctype.h>
#include "tinyexpr.h"

char buf[BUFSIZ];
int err;
struct passwd *pw;
uid_t uid;

void help(void);
void string_input(char *input);
void graph(bool audio);

int main(int argc, char *argv[])
{
    double sum, result, count=1;
    sum = result = 0;
    te_variable vars[] = {{"t", &sum}, {"r", &result}, {"c", &count}};

    uid = getuid();
    pw = getpwuid(uid);

    printf(" +------------+\n"
           " | calculator |\n"
           " +------------+\n");
    while (1)
    {
        printf("\033[33m+calc+\033[0m:%s\033[1;34m(\033[0m%u\033[1;34m)\033[0;37;1m $\033[0;33m ", pw->pw_name, (unsigned)count++);
        string_input(buf);
        printf("\033[0m");

        if (strlen(buf) == 0)
        {
            continue;
        }
        if (strcmp(buf, "graph") == 0)
        {
            graph(false);
            continue;
        }
        if (strcmp(buf, "ls") == 0 || strcmp(buf, "cd") == 0)
        {
            printf("\tYou are not in your standard shell \033[1;32m;-)\033[0m\n");
            continue;
        }
        if (strcmp(buf, "help") == 0)
        {
            help();
            continue;
        }
        if (strcmp(buf, "clear") == 0)
        {
            printf("\033c");
            continue;
        }
        if (strcmp(buf, "exit") == 0 || strcmp(buf, "q") == 0)
        {
            exit(0);
            continue;
        }
        te_expr *expr = te_compile(buf, vars, 3, &err);
        if (expr)
        {
            result = te_eval(expr);
            sum = sum + result;
            printf("\t\033[1mresult\033[0m: \033[33m%.18f\033[0m\n\t\033[1mtotal\033[0m:  %.18f\n", result, sum);
            te_free(expr);
        }
        else
        {
            printf("\terror parsing expression at: %d\n", err);
        }
    }
    return 0;
}

void help()
{
    printf(
        "\n +-------------+\n"
        " | quick intro |\n"
        " +-------------+\n"
        " \033[33mnumbers\033[0m are just normal: \033[1;35m45\033[0m, \033[1;35m3.14159\033[0m, \033[1;35m100000\033[0m, ...\n\n"
        " \033[33moperators\033[0m: \033[1;35m+\033[0m, \033[1;35m-\033[0m, \033[1;35m*\033[0m, \033[1;35m/\033[0m, \033[1;35m%%\033[0m (modulo), ...\n"
        "\t-> for example: \033[1;35m4 * 4\033[0m, \033[1;35m13 + (4*5)\033[0m\n\n"
        " \033[33mfunctions\033[0m: \033[1;35msqrt\033[0m, \033[1;35msin\033[0m, \033[1;35mcos\033[0m, \033[1;35mfloor\033[0m, ...\n"
        "\t-> use this way: \033[1;35msinh(4)\033[0m or without brackets: \033[1;35mlog 4.6543\033[0m\n\n"
        " \033[33mconstants\033[0m: \033[1;35mpi\033[0m, \033[1;35me\033[0m (euler)\n\n"
        " environment \033[33mvariables\033[0m: \033[1;35mr\033[0m, \033[1;35mt\033[0m, \033[1;35mc\033[0m\n"
        "\t-> \033[1;35mr\033[0m represents the result of the last calculation.\n"
        "\t-> \033[1;35mt\033[0m is the total of all results in the session.\n"
        "\t-> \033[1;35mc\033[0m is the count of evaluations.\n\n"
        " environment \033[33mcommands\033[0m: \033[1;35mhelp\033[0m, \033[1;35mexit\033[0m, \033[1;35mq\033[0m, \033[1;35mclear\033[0m\n\n"
        "\t-> \033[1;35mhelp\033[0m this page (WOW)\n"
        "\t-> \033[1;35mexit\033[0m or \033[1;35mq\033[0m has the same effect as \033[1;35m^C\033[0m - It quits the application!\n"
        "\t-> \033[1;35mclear\033[0m purge console screen\n\n"
        " That\'s it basically ...\n\n"
        " +-------------+\n"
        " | information |\n"
        " +-------------+\n"
        " \033[4;33mhttps://github.com/lukas-h/calc\n\033[0m \033[40;37mLicense \033[1;43;30mMIT\033[0m\n\n"
        " Built with tinyexpr (\033[4mhttps://github.com/codeplea/tinyexpr\033[0m)\n - there you\'ll find more documentation.\n\n");
}
void string_input(char *input)
{
    unsigned i = 0;
    char k;
    while ((k = getchar()) != '\n')
    {
        if (isupper(k))
        {
            input[i++] = tolower(k);
        }
        else
        {
            input[i++] = k;
        }
    }
    input[i] = '\0';
}
void graph(bool audio)
{
    printf("\t\033[33;1m+graph+\033[0m:%s\033[1;34m(\033[33m>>>\033[1;34m)\033[0;37;1m $\033[0;33m ", pw->pw_name);
    string_input(buf);
    printf("\033[0m");

    double periods = 2.0, i;
    te_variable vars[] = {{"x", &i}, {"periods", &periods}};

    te_expr *expr = te_compile(buf, vars, 1, &err);
    if (expr)
    {
        for (i = 0.0; round(i) <= (periods * M_PI); i += (M_PI / 6.0))
        {
            double k = -4.0, j = -3.9, v;
            int c, r = 0;
            bool match = false;

            v = te_eval(expr);

            for (c = 0; j != 4.0 && c < 80; k += 0.1, j += 0.1, c++)
            {
                if (v >= k && v <= j)
                {
                    match = true;
                    break;
                }
            }
            if (match)
            {
                while (r++ < 80)
                {
                    if (r == c)
                    {
                        printf("\033[31mx\033[0m");
                    }
                    if (r == 38 && c <= 38)
                    {
                        putchar('|');
                    }
                    if (r == 39 && c > 39)
                    {
                        putchar('|');
                    }
                    else
                    {
                        putchar(' ');
                    }
                }
                putchar('\n');
            }
            else
            {
                while (c-- != 42)
                {
                    putchar(' ');
                }
                printf("| can\'t show number\n");
            }
        }
    }
    else
    {
        printf("\terror parsing expression at: %d\n", err);
    }
}