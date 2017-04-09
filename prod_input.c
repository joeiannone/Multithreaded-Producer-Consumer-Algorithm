#include	<stdio.h>

// Input data generator for CS 341 PA2.  BKR

int
isPrime( int n )
{
	int     i;

	for ( i = 2 ; i*i <= n ; i++ )
	{
		if ( n % i == 0 )
		{
			return 0;
		}
	}
	return 1;
}


static int
isfib( int i )
{
	static int	x = 1, y = 1;
	int		z;

	if ( i == y )
	{
		z = x + y;
		x = y;
		y = z;
		return 1;
	}
	else
	{
		return 0;
	}
}

is3or5( int i )
{
	if ( i % 3 == 0 )	return 1;
	else if ( i % 5 == 0 )	return 1;
	else			return 0;
}

int
main()
{
	int		i;

	for ( i = 0 ;; i++ )
	{
		if ( isPrime( i ) )
		{
			printf( "red %d\n", i );
		}
		if ( isfib( i ) )
		{
			printf( "green %d\n", i );
		}
		if ( is3or5( i ) )
		{
			printf( "blue %d\n", i );
		}
	}
	return 0;
}
