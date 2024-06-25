#include "../../Includes/minishell.h"
#include <limits.h>

void	ft_pwd(void)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, PATH_MAX) == NULL)
	{
		printf("Error: Isn't possible to get the current directory\n");
		return ;
	}
	printf("%s\n", pwd);
}
