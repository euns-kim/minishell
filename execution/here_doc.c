/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarts <tmarts@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 16:34:55 by tmarts            #+#    #+#             */
/*   Updated: 2023/07/28 18:34:17 by tmarts           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	here_doc(char *delimiter)
{
	int		here_doc_fd;
	char	*line;

	here_doc_fd = open("heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (here_doc_fd == -1)
		return (announce_error("heredoc", "Failed to open file", 2), -1);
	line = readline("> ");
	while (line && ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) != 0)
	{
		ft_putendl_fd(line, here_doc_fd);
		free(line);
		line = readline("> ");
	}
	free(line);
	close(here_doc_fd);
	here_doc_fd = open("heredoc.tmp", O_RDONLY, 0644);
	if (here_doc_fd == -1)
		return (announce_error("heredoc", "Failed to open file", 2), -1);
	if (unlink("heredoc.tmp") == -1)
		return (announce_error("heredoc", "Failed to unlink", 2), -1);
	return (here_doc_fd);
}

t_redirect	*here_doc_all(int *here_doc_fd, t_redirect *stdin_redirect)
{
	t_redirect	*tmp;
	t_redirect	*heredoc_node;
	int			i;

	i = 0;
	heredoc_node = NULL;
	tmp = stdin_redirect;
	while (tmp != NULL)
	{
		if (tmp->type == REDIRECT_HERE_DOC)
		{
			if (i > 0)
				close(*here_doc_fd);
			heredoc_node = tmp;
			*here_doc_fd = here_doc(tmp->word);
			if (*here_doc_fd < 0)
				return (heredoc_node);
			i++;
		}
		tmp = tmp->next;
	}
	return (heredoc_node);
}
