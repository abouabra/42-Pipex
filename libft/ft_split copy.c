/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 10:38:10 by abouabra          #+#    #+#             */
/*   Updated: 2022/11/28 19:45:54 by abouabra         ###   ########.fr       */
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
				while(str[i] && str[i] == '\'')
				{
					i++;
					int len = ft_strchr_number((char *)&str[i], '\'');
					if(len == 0)
						break;
					i += len-1;
				}
				// if(str[i] == '\"')
				// {
				// // 	//printf("IN: %s\n",&str[i]);
				// 	i++;
				// 	i += ft_strchr_number((char *)&str[i],'\"');
				// 	while(str[i-1] == '\\')
				// 	{
				// 	 	i++;
				// 		i += ft_strchr_number((char *)&str[i],'\"');
				// 	}
				// // 	//printf("OUT: %s\n",&str[i]);
				// }
				while(str[i] && str[i] == '\"')
				{
					i++;
					int len = ft_strchr_number((char *)&str[i], '\"');
					if(len == 0)
						break;
					i += len-1;
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
	// //printf("COUNTER: %d\n",counter);
	str = ft_calloc((wdlen + 1) , (sizeof(char)));
	if (!str)
		return (0);
	i = 0;
	int decider = 0;
	while (s[(*index)] && s[(*index)] != c)
	{
		if(s[(*index)] == '\"' )
		{
			// //printf("COUNTER2: %d    COUNTER: %d\n",counter2,counter);
			// if(counter==1)
			// {
			// 	str[i] = s[*index];
			// 	i++;
			// }
			// if(s[(*index)] == '\"' && ((counter2 > 1 && counter2 < counter) || 0))//counter2 % 2 == 0))
			// {
			// 	str[i] = s[*index];
			// 	i++;
			// }
			// if(s[(*index)] == '\"')//counter2 % 2 == 0))
			// {
			// 	// if(s[(*index)] == '\"' && ((counter2 > 1 && counter2 < counter) || 0))//counter2 % 2 == 0)
			// 	// {
			// 	// 	//str[i] = s[*index];
			// 	// 	i++;
			// 	// }
			// 	// else
			// 	// {
			// 		str[i] = s[*index];
			// 		i++;
			// 	// }
				
			// }
			// counter2++;

			// (*index)++;

			
			// // printf("IN: |%s|\n",&s[(*index)]);
			// //str[i] = s[*index];
			// //i++;
			// //(*index)++;
		 	// ft_strlcpy(&str[i],&s[(*index)],gg+1);
			// (*index) += gg-1;
			// i += gg-1;
			// // printf("STRLCPY: |%s|\n",&s[(*index)]);
			// while(s[(*index)] == '\\' || s[(*index)-1] == '\\')
			// {
			// 	if(s[(*index)-1] == '\\')
			// 	{
			// // 		printf("STRLCAT1: |%s| |%s|\n",&s[(*index)],&str[i]);
			// 		i++;
			// 		str[i] = s[*index];
			// 		//(*index)++;
			// 	}
			// 	gg = ft_strchr_number((char *)&s[i],'\"');
		 	// 	ft_strlcat(&str[i],&s[(*index)],gg+1);
			// // 	//printf("STRLCAT2: |%s|\n",&s[(*index)]);
			// }
			while(s[*index] && s[*index] == '\"')
			{
				str[i] = s[*index];
				i++;
				(*index)++;
				int len = ft_strchr_number((char *)&s[*index], '\"');
				// //printf("STRCHR: %s\n",&s[*index + len -1]);
				if(len == 0)
					break;
				char *end = ft_substr(&s[*index], 0, len);
				//ft_strjoin(&str[i], end);
				//ft_strjoin(ft_strdup(str), end);
				// //printf("STR BEFORE: %s\n",&s[*index]);
				// printf("END: %d\n",len);
				ft_strlcat(&str[i], end, len +1);
				// printf("STR After: %s\n",str);
				// // printf("LEN: %d | INDEX: %d | END: %s | STR: %s|\n\n",len,*index,end,str);
				(*index) += len-1;
				i += len-1;
				// while(s[*index] && i < len)
				// {
				// 	i++;
				// 	(*index)++;
				// }
			}
			decider = 1;
			// printf("GG: %s\n",str);
		}
		else if(s[(*index)] == '\'')
		{
			while(s[*index] && s[*index] == '\'')
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
	// //printf("GGGGGG   %s\n",str);
	if(decider == 1)
	{
		// //printf("COMMAND: %s\n",str);
		int fff = ft_strchr_number(str, '\"');
		if(fff == 0)
			return ft_strtrim(str, "\"");
		char *tmp_str = ft_strtrim(&str[fff], "\"");
		// //printf("%d %s %s\n",fff,&str[fff],tmp_str);
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
		// //printf("%d %s %s\n",fff,&str[fff],tmp_str);
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
	index = 0;
	i = 0;
	c_occurence = c_count(s, c);
	// //printf("COUNT: %d\n",c_occurence);
	// //printf("LEN OF S: %d\n", (int)ft_strlen(s));
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

// int main(int ac, char **av)
// {
// 	char **str;
// 	int i=0;
// 	//str = ft_split(av[1], ' ');
// 	//str = ft_split("awk  \"./script\"quote.sh\"", ' ');
// 	//str = ft_split("./script\"quote.sh", ' ');
// // 	//str = ft_split("awk '{count++} END {printf \\\"count: %i\\\" , count}'", ' ');
// 	//str = ft_split("awk \"{count++} END  {print count}\"", ' ');
// // 	//str = ft_split("awk \"{count++} END {printf \\\"count: gg\\\" , count}\"", ' ');
// 	//str = ft_split("sed \"s/And/But/\"", ' ');
// 	str = ft_split(av[1], ' ');
// 	//str = ft_split("awk \"{count++} END {printf \\\"count: %i\\\" , count}\"", ' ');

// 	//str = ft_split("./\"script space.sh\"", ' ');
// 	//str = ft_split("./'script space.sh'", ' ');
// 	//str = ft_split("awk '\"{count++} END {print count}\"'", ' ');
// 	while(str[i])	
//  		printf("|%s|\n",str[i++]);
// 	return 0;
// }