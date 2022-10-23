begin
	writeln (' type M and N ' );
	readln ( M, N );
	while M <> N do
	begin
		if M>N
			then M := M-N
			else N := N - M
	end;
	write ('NOD=', M )
end.