/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 13:26:06 by abouabra          #+#    #+#             */
/*   Updated: 2022/10/02 13:26:07 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	while (*(unsigned char *)s && *(unsigned char *)s != (unsigned char)c)
		s++;
	if (*(unsigned char *)s == (unsigned char)c)
		return ((char *)s);
	return (0);
}
