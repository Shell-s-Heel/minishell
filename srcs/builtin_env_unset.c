#include "minishell.h"

int			env_builtin(t_list **env, t_command *cmd)
{
	if ((*env)->next == NULL)
		g_exit_status = 0;
	if ((*env)->next != NULL)
		env_builtin(&((*env)->next), cmd);
	ft_putstr_fd(ENV_KEY(*env), cmd->fd[1]);
	ft_putstr_fd(ENV_VALUE(*env), cmd->fd[1]);
	ft_putchar_fd('\n', cmd->fd[1]);
	return (0);
}

int			export_unset_error(t_list **env, t_command *cmd, t_list **export)
{
	size_t	i;
	int		ret;

	i = 0;
	ret = -1;
	while (cmd->command[1][i] && (i < ft_strclen(cmd->command[1], '=')))
	{
		if (cmd->command[1][i] == '_')
			i++;
		else if ((ret = ft_isalnum((char)cmd->command[1][i])) == 0)
			break ;
		else
			i++;
	}
	if ((ft_isdigit(cmd->command[1][0]) || ret == 0 || cmd->command[1][0] == '='
		|| (ft_strcmp(&cmd->command[0][0], "unset") == 0 &&
		ft_strchr(&cmd->command[1][0], '='))) && cmd->command[1][0] != '-')
	{
		error_msg("bash", cmd, cmd->command[1], "not a valid identifier");
		g_exit_status = 1;
		return (RT_FAIL);
	}
	else if (ft_strcmp(&cmd->command[0][0], "export") == 0)
		export_builtin_arg(env, export, cmd);
	return (0);
}

int			unset_builtin(t_list **env, t_command *cmd, t_list **export)
{
	int		i;

	i = 0;
	update_underscore(env, last_arg(cmd));
	if (cmd->command[0] && cmd->command[1])
	{
		while (cmd->command[++i])
		{
			delete_env_variable(env, cmd->command[i]);
			delete_env_variable(export, cmd->command[i]);
			g_exit_status = 0;
		}
		if (export_unset_error(env, cmd, export) == RT_FAIL)
			return (RT_FAIL);
		else if (cmd->command[1][0] == '-')
		{
			error_msg("bash", cmd, "-", "invalid option");
			g_exit_status = 2;
		}
	}
	return (RT_SUCCESS);
}
