#include "minishell.h"

static void	update_export_underscore(t_list **env, t_command *cmd)
{
	char	*last;
	char	*needle;
	int		len;

	last = last_arg(cmd);
	if (!(needle = ft_strchr(last, '=')))
		len = ft_strlen(last);
	else
		len = needle - last;
	if (!(needle = ft_substr(last, 0, len)))
		return ;
	update_underscore(env, needle);
	free(needle);
}

static int	check_export_arg(char *arg, t_command *cmd)
{
	int		ret;
	int		end;
	int		i;

	end = ft_strclen(arg, '=');
	i = -1;
	ret = 1;
	while (arg[++i] && i < end)
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			ret = 0;
	if (ft_isdigit(*arg) || !ret || !end)
	{
		error_msg("bash", cmd, arg, "not a valid identifier");
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

static int	export_builtin_arg(t_list **env, t_list **export, t_command *cmd)
{
	int		i;

	i = -1;
	while (cmd->command[++i])
	{
		if (!check_export_arg(cmd->command[i], cmd))
			continue;
		g_exit_status = 0;
		if (ft_strchr(&cmd->command[i][0], '=') != NULL)
		{
			add_env_variable(env, &cmd->command[i][0]);
			add_env_variable(export, &cmd->command[i][0]);
		}
		else if (ft_strchr(&cmd->command[i][0], '=') == NULL)
			add_env_variable(export, &cmd->command[i][0]);
	}
	return (RT_SUCCESS);
}

int			export_builtin(t_list **env, t_command *cmd, t_list **export)
{
	char	**export_tab;
	char	buf[3];

	update_export_underscore(env, cmd);
	if (cmd->command[0] && !cmd->command[1])
	{
		if (!(export_tab = env_list_to_tab(*export)))
			return (RT_FAIL);
		print_export(export_tab, cmd->fd);
		ft_freetab(export_tab);
	}
	buf[2] = 0;
	if (cmd->command[1] && cmd->command[1][0] == '-')
	{
		error_msg("bash", cmd, ft_strncpy(buf, cmd->command[1], 2),
				"invalid option");
		g_exit_status = 2;
		return (RT_SUCCESS);
	}
	if (cmd->command[0] && cmd->command[1])
		export_builtin_arg(env, export, cmd);
	return (RT_SUCCESS);
}
