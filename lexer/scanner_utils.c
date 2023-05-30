/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eunskim <eunskim@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 14:35:52 by eunskim           #+#    #+#             */
/*   Updated: 2023/05/30 19:35:25 by eunskim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	advance(t_scanner *scanner)
{
	char	c;

	c = *scanner->current;
	if (c != '\0')
		scanner->current++;
	return (c);
}

void	init_scanner(t_scanner *scanner, const char *source)
{
	scanner->start = source;
	scanner->current = source;
}
