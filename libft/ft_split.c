/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 10:38:10 by abouabra          #+#    #+#             */
/*   Updated: 2022/11/27 23:04:47 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_strchr_number(char *s, int c)
{
	int i;

	i=0;
	while (s[i] && s[i] != c)
	{
		i++;		
	}
	if (s[i] == c)
		return (i);
	return (0);
}


static char	**error(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

static int	c_count(char const *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			i++;
		else
		{
			count++;
			while (str[i] && str[i] != c)
			{
				if(str[i] == '\'')
				{
					i++;
					i += ft_strchr_number((char *)&str[i],'\'');
					if(str[i-1] == '\\')
					{
					 	i++;
						i += ft_strchr_number((char *)&str[i],'\'');
					}
				}
				if(str[i] == '\"')
				{
					i++;
					i += ft_strchr_number((char *)&str[i],'\"');
					if(str[i-1] == '\\')
					{
					 	i++;
						i += ft_strchr_number((char *)&str[i],'\"');
					}
					//printf("##%s###\n",&str[i]);
				}
				i++;
			}
		}
	}
	return (count);
}

static char	*get_word(char const *s, char c, int *index)
{
	int		i;
	int		wdlen;
	char	*str;
	int gg;
	wdlen = 0;
	i = 0;
	while (s[(*index)] == c)
		(*index)++;
	i = *index;
	while (s[i] && s[i] != c)
	{
		if(s[i] == '\"')
		{
			i++;
			wdlen++;
			gg = ft_strchr_number((char *)&s[i],'\"');
			wdlen += gg;
			i += gg;
			if(s[i-1] == '\\')
			{
				i++;
				gg += ft_strchr_number((char *)&s[i],'\"');
				wdlen += gg;
				i += gg;
			}
		}
		if(s[i] == '\'')
		{
			i++;
			wdlen++;
			gg = ft_strchr_number((char *)&s[i],'\'');
			wdlen += gg;
			i += gg;
			if(s[i-1] == '\\')
			{
				i++;
				gg += ft_strchr_number((char *)&s[i],'\'');
				wdlen += gg;
				i += gg;
			}
		}
		wdlen++;
		i++;
	}
	str = malloc((wdlen + 1) * (sizeof(char)));
	if (!str)
		return (0);
	i = 0;
	while (s[(*index)] && s[(*index)] != c)
	{
		if(s[(*index)] == '\"')
		{
			//str[i] = s[*index];
			//i++;
			(*index)++;
		 	ft_strlcpy(&str[i],&s[(*index)],gg+1);
			(*index) += gg;
			i += gg;
			if(s[(*index)] == '\\')
			{
				//(*index)++;
				gg = ft_strchr_number((char *)&str[i],'\"');
		 		ft_strlcat(&str[i],&s[(*index)],gg+1);
			}
		}
		if(s[(*index)] == '\'')
		{
			//str[i] = s[*index];
			//i++;
			(*index)++;
		 	ft_strlcpy(&str[i],&s[(*index)],gg+1);
			(*index) += gg;
			i += gg;
			if(s[(*index)] == '\\')
			{
				//(*index)++;
				gg = ft_strchr_number((char *)&str[i],'\'');
		 		ft_strlcat(&str[i],&s[(*index)],gg+1);
			}
		}
		if(s[(*index)] != '\"' && s[(*index)] != '\'')
			str[i] = s[*index];
		i++;
		(*index)++;
	}
	str[i] = 0;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	int		c_occurence;
	int		i;
	char	**arr;
	int		index;

	if (s == NULL)
		return (0);
	index = 0;
	i = 0;
	c_occurence = c_count(s, c);
	arr = malloc(sizeof(char *) * (c_occurence + 1));
	if (!arr)
		return (0);
	while (i < c_occurence)
	{
		arr[i] = get_word(s, c, &index);
		if (!arr)
			return (error(arr));
		i++;
	}
	arr[i] = 0;
	return (arr);
}

int main()
{
	char **str;
	int i=0;
	str = ft_split("tr './script\\\"quote.sh' ", ' ');
	while(str[i])	
		printf("|%s|\n",str[i++]);
	return 0;
}