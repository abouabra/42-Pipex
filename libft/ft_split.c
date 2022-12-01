/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 10:38:10 by abouabra          #+#    #+#             */
/*   Updated: 2022/12/01 13:16:10 by abouabra         ###   ########.fr       */
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
					while(str[i-1] == '\\')
					{
					 	i++;
						i += ft_strchr_number((char *)&str[i],'\'');
					}
				}
				if(str[i] == '\"')
				{
					//printf("IN: %s\n",&str[i]);
					i++;
					i += ft_strchr_number((char *)&str[i],'\"');
					while(str[i-1] == '\\')
					{
					 	i++;
						i += ft_strchr_number((char *)&str[i],'\"');
					}
					//printf("OUT: %s\n",&str[i]);
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
	int counter=1;
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
				//counter++;
			}
			counter++;
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
	//printf("COUNTER: %d\n",counter);
	str = malloc((wdlen + 1) * (sizeof(char)));
	if (!str)
		return (0);
	i = 0;
	int decider = 0;
	while (s[(*index)] && s[(*index)] != c)
	{
		if(s[(*index)] == '\"' )
		{
			while(s[*index] == '\"')
			{
				str[i] = s[*index];
				i++;
				(*index)++;
				int len = ft_strchr_number((char *)&s[*index], '\"');
				if(len == 0)
					break;
				char *end = ft_substr(&s[*index], 0, len);
				ft_strlcat(&str[i], end, len +1);
				(*index) += len-1;
				i += len-1;
			}
			decider = 1;
		}
		else if(s[(*index)] == '\'')
		{
			while(s[*index] == '\'')
			{
				str[i] = s[*index];
				i++;
				(*index)++;
				int len = ft_strchr_number((char *)&s[*index], '\'');
				if(len == 0)
					break;
				char *end = ft_substr(&s[*index], 0, len);
				ft_strlcat(&str[i], end, len +1);
				(*index) += len-1;
				i += len-1;
			}
			decider = 2;
		}
		if(s[(*index)] != '\"' && s[(*index)] != '\'')
			str[i] = s[*index];
		i++;
		(*index)++;
	}
	//printf("GGGGGG   %s\n",str);
	if(decider == 1)
	{
		//printf("COMMAND: %s\n",str);
		int fff = ft_strchr_number(str, '\"');
		if(fff == 0)
			return ft_strtrim(str, "\"");
		char *tmp_str = ft_strtrim(&str[fff], "\"");
		printf("%d %s %s\n",fff,&str[fff],tmp_str);
		ft_memmove(&str[fff], tmp_str, ft_strlen(tmp_str));
		str[fff] = 0;
		str = ft_strjoin(ft_strdup(str), tmp_str);
	}
	if(decider == 2)
	{
		int fff = ft_strchr_number(str, '\'');
		if(fff == 0)
			return ft_strtrim(str, "\'");
		char *tmp_str = ft_strtrim(&str[fff], "\'");
		printf("%d %s %s\n",fff,&str[fff],tmp_str);
		ft_memmove(&str[fff], tmp_str, ft_strlen(tmp_str));
		str[fff] = 0;
		str = ft_strjoin(ft_strdup(str), tmp_str);
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
	if(!ft_strncmp(s, "./", 2))
	{
		arr = malloc(sizeof(char *)*2);
		if(!arr)
			return 0;
		arr[0] = ft_strdup(s);
		arr[1] = NULL;
		return arr;
	}
	index = 0;
	i = 0;
	c_occurence = c_count(s, c);
	//printf("COUNT: %d\n",c_occurence);
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

// int main()
// {
// 	char **str;
// 	int i=0;
// 	//str = ft_split(av[1], ' ');
// 	//str = ft_split("awk  \"./script\"quote.sh\"", ' ');
// 	//str = ft_split("./script\"quote.sh", ' ');
// 	//str = ft_split("awk '{count++} END {printf \\\"count: %i\\\" , count}'", ' ');
// 	//str = ft_split("awk \"{count++} END  {print count}\"", ' ');
// 	//str = ft_split("awk \"{count++} END {printf \\\"count: gg\\\" , count}\"", ' ');
// 	//str = ft_split("sed \"s/And/But/\"", ' ');
	
// 	//str = ft_split("./\"script space.sh\"", ' ');
// 	str = ft_split("./script space.sh", ' ');
// 	//str = ft_split("awk '\"{count++} END {print count}\"'", ' ');
// 	while(str[i])	
// 		printf("|%s|\n",str[i++]);
// 	return 0;
// }