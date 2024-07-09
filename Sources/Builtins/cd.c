#include "../../Includes/minishell.h"
#include <limits.h>

int countTokens(t_data *data) {
    int count = 0;

    t_token *current = data->token;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count - 1;
}

char	*ft_go_to(char *directory_path)
{
	char	*current_dir;
	char	buff[PATH_MAX];

	current_dir = getcwd(buff, PATH_MAX); // Get the current directory [OK]
	if (chdir(directory_path) == -1) // Change the directory to the desired directory [OK]
		printf("minishell: cd: %s: No such file or directory\n", directory_path);
	else {
		// TODO: Set the OLDPWD variable to the current directory into data->envp
		// EXTRA: We can use access to check if the route is available or not
	}
	current_dir = getcwd(buff, PATH_MAX); // Get the current directory [OK]
	return (current_dir);
}

char	*ft_get_env_dir(t_data *data, char *d_dir) // d_dir == Desired Directory [OK]
{
	int i = 0;
	char *tmp_dir;

	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], d_dir, ft_strlen(d_dir)))
		{
			tmp_dir = ft_strdup(data->envp[i] + ft_strlen(d_dir) + 1);
			return (tmp_dir);
		}
		i++;
	}
	return (NULL);
}

void    ft_cd(t_data *data)
{
	int token_counter;
	char *desired_path;
	char *home_dir;

	token_counter = countTokens(data); // Get the number of arguments [OK]
	desired_path = data->token->next->content; // Get the desired path [OK]
	home_dir = ft_get_env_dir(data, "HOME"); // Get the home directory [OK]

	//printf("TokenCounter: %d\n", token_counter); // Debug
	//printf("DesiredPath: %s\n", desired_path); // Debug
	//printf("HomeDir: %s\n", home_dir); // Debug

	if (token_counter > 2) // If there are more than one argument, print an error [OK]
		ft_print_exit("minishell: cd: too many arguments\n");
	if (!desired_path) // If there is no argument, go to the home directory [OK]
		ft_go_to(home_dir);
	else if (desired_path[0] == '-' && !desired_path[1]) // If the argument is "-", go to the previous directory [OnDoing]
		ft_go_to("OLDPWD");
	else // If there is an argument, go to the desired directory [OK]
		ft_go_to(desired_path);
}
