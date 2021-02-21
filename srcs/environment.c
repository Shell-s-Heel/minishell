#include "minishell.h"

static int	create_env_struct(char *keyvalue, t_env *env)
{
	char	*needle;

	if (!(needle = ft_strrchr(keyvalue, '=')))
		return (0);
	env->key = ft_substr(keyvalue, 0, needle - keyvalue);
	env->value = ft_strdup(needle + 1);
	return (1);
}

t_list	*create_env_list(char **envp)
{
	t_env	*new;
	t_list	*env;
	t_list	*tmp_env;

	env = NULL;
	while (*envp != NULL)
	{
		new = (t_env*)malloc(sizeof(t_env));
		create_env_struct(*envp, new);
		tmp_env = ft_lstnew(new);
		ft_lstadd_front(&env, tmp_env);
		envp++;
	}
	return (env);
}

char	**env_list_to_tab(t_list *env)
{
	int		count;
	t_list	*tmp_env;
	char	**new_env;
	char	*tmp_str;

	count = 1;
	tmp_env = env;
	while ((tmp_env = tmp_env->next))
		count++;
	new_env = (char **)malloc(sizeof(char *) * (count + 1));
	ft_bzero(new_env, count + 1);
	while (--count >= 0 && env != NULL)
	{
		tmp_str = ft_strjoin(ENV_KEY(env), "=");
		new_env[count] = ft_strjoin(tmp_str, ENV_VALUE(env));
		free(tmp_str);
		env = env->next;
	}
	return (new_env);
}

void	add_env_variable(t_list **env, char *var)
{
	t_env	*new;
	t_list	*tmp_env;

	new = (t_env*)malloc(sizeof(t_env));
	if (!create_env_struct(var, new))
			return ;
	tmp_env = *env;
	while (tmp_env)
	{
		if (!ft_strcmp(new->key, ENV_KEY(tmp_env)))
		{
			free(new->key);
			free(ENV_VALUE(tmp_env));
			ENV_VALUE(tmp_env) = new->value;
			break;
		}
		tmp_env = tmp_env->next;
	}
	if (tmp_env == NULL)
	{
//		new = (t_env*)malloc(sizeof(t_env));
		tmp_env = ft_lstnew(new);
		ft_lstadd_front(env, tmp_env);
	}
	free(new);
}

char	*find_env_value(t_list **env, char *key)
{
	t_list	*tmp_env;

	tmp_env = *env;
	while (tmp_env)
	{
		if (!ft_strcmp(key, ENV_KEY(tmp_env)))
			return (ENV_VALUE(tmp_env));
		tmp_env = tmp_env->next;
	}
	return (NULL);
}
