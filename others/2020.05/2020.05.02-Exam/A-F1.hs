fun::Integer->Integer->Integer
fun i n= case compare i n of
	LT-> cv*cv+fun (i*2) n
		where cv=(n-i) `div` (i*2)+1
	_ ->  0

main::IO()
main=do
	n<-getLine
	putStrLn (show (fun 1 (read n)))