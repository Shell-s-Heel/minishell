#include "minishell.h"

static int	echo_n_parser(char *str)
{
	if (*str != '-')
		return (1);
	while (*(str + 1))
	{
		if (*(str + 1) != 'n')
			return (1);
		str++;
	}
	return (0);
}

int			echo_builtin(t_list **head, t_command *cmd)
{
	int		flag;

	flag = 0;
	(void)head;
	cmd->command++;
	while (!echo_n_parser(*(cmd->command)))
	{
		flag = 1;
		cmd->command++;
	}
	while (*(cmd->command) != NULL)
	{
		ft_putstr_fd(*(cmd->command), cmd->fd[1]);
		ft_putchar_fd(' ', cmd->fd[1]);
		cmd->command++;
	}
	if (!flag)
		ft_putchar_fd('\n', cmd->fd[1]);
	return (0);
}

int			env_builtin(t_list **head, t_command *cmd)
{
	char	**env;
	char	**ptr;

	env = env_list_to_tab(*head);
	ptr = env;
	while (*env != NULL)
	{
		ft_putstr_fd(*env, cmd->fd[1]);
		ft_putchar_fd('\n', cmd->fd[1]);
		free(*env);
		env++;
	}
	free(ptr);
	return (0);
}

int			export_builtin(t_list **head, t_command *cmd)
{
	if (cmd->command[0])
		add_env_variable(head, cmd->command[1]);
	return (0);
}

int			unset_builtin(t_list **head, t_command *cmd)
{
	if (cmd->command[0])
		delete_env_variable(head, cmd->command[1]);
	return (0);
}