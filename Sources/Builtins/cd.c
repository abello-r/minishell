#include "../../Includes/minishell.h"
#include <limits.h>

char	*ft_go_to(char *directory_path)
{
	char	*current_dir;
	char	buff[PATH_MAX];

	current_dir = getcwd(buff, PATH_MAX); // Get the current directory [OK]
	printf("Current Directory [OLD]: %s\n", current_dir); // Debug

	if (chdir(directory_path) == -1) // Change the directory to the desired directory [OK]
	{
		ft_print_exit("minishell: cd: No such file or directory\n");
	}
	else {
		// TODO: Set the OLDPWD variable to the current directory into data->envp
		// EXTRA: We can use access to check if the route is available or not
	}

	current_dir = getcwd(buff, PATH_MAX); // Get the current directory [OK]
	printf("Current Directory [NEW]: %s\n", current_dir); // Debug

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
			printf("tmp_dir: %s\n", tmp_dir); // Debug
			return (tmp_dir);
		}
		i++;
	}
	return (NULL);
}

void    ft_cd(t_data *data)
{
	int tmp_argc = 0; // Replace with data->argc [Should be the number of arguments]

	char *tmp_desired_input; // Replace with data->input [Should be a "cd" as type command]
	tmp_desired_input = "cd"; 

	char *tmp_desired_args; // Replace with data->args [Should be a path to a directory]
	tmp_desired_args = NULL;

	char *home_dir; // [OK]
	home_dir = ft_get_env_dir(data, "HOME");

	printf("Input: %s\n", tmp_desired_input); // Debug
	printf("Args: %s\n", tmp_desired_args); // Debug

	if (tmp_argc > 1) // If there are more than one argument, print an error [OK]
	{
		ft_print_exit("minishell: cd: too many arguments\n");
	}

	if (!tmp_desired_args) // If there is no argument, go to the home directory [OK]
	{
		ft_go_to(home_dir);
	}
	else if (ft_strncmp(&tmp_desired_args[0], "-", 1)) // If the argument is "-", go to the previous directory [CHECK]
	{
		ft_go_to("OLDPWD");
	}
	else // If there is an argument, go to the desired directory [CHECK]
	{
		ft_go_to(tmp_desired_args);
	}

}
