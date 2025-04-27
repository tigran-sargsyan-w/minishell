/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsargsya <tsargsya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:08:36 by tsargsya          #+#    #+#             */
/*   Updated: 2025/04/27 14:27:33 by tsargsya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// function to test gnl from test.txt
void	test_gnl(void)
{
	int		fd;
	char	*line;

	printf("TESTING GNL\n");
	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		return ;
	}
	line = get_next_line(fd);
	printf("First line: %s", line);
	line = get_next_line(fd);
	if (line == NULL)
	{
		printf("No line found\n");
		free(line);
		close(fd);
		return ;
	}
	printf("Second line: %s", line);
	free(line);
	close(fd);
}

// function to test ft_printf
void	test_printf(void)
{
	int		num;
	char	*str;

	num = 42;
	str = "Hello, World!";
	printf("\nTESTING FT_PRINTF\n");
	ft_printf("Integer: %d\n", num);
	ft_printf("String: %s\n", str);
}

// function to test libft functions
void	test_libft(void)
{
	char	*str;

	str = "Hello, World!";
	printf("\nTESTING LIBFT\n");
	printf("Length of string: %zu\n", ft_strlen(str));
	printf("Is 'H' alpha: %d\n", ft_isalpha('H'));
	printf("Is 'H' digit: %d\n", ft_isdigit('H'));
}

int	main(void)
{
	test_gnl();
	test_printf();
	test_libft();
	printf("\nWelcome to the shell!\n");
	return (0);
}
