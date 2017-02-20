#!/bin/awk

{ 
	if ($1=="malloc") 
	{
		addr[$2]=NR
		size[$2]=$6
		from[$2]=$4
	}
	if ($1=="free")
	{
		if(!addr[$2]) 
		{
			print "fail free " $2 " from " $4	
		}
		else{
			delete addr[$2]
		}
	}
        pos=match($1,/^\*+/)
        if(pos>0) 
	{
		addr[$0","NR]=NR 
	}
}
END{
	for(i in addr)
	{
		if(addr[i]>=startline)
		print i,addr[i],from[i],size[i]|"sort -n -k2";
	} 
}



