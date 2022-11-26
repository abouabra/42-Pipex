/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 13:26:21 by abouabra          #+#    #+#             */
/*   Updated: 2022/10/05 16:24:29 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int	i;

	i = ft_strlen(str);
	while (i != 0 && ((unsigned char *)str)[i] != (unsigned char)c)
		i--;
	if (((unsigned char *)str)[i] == (unsigned char)c)
		return ((char *)&str[i]);
	return (0);
}
