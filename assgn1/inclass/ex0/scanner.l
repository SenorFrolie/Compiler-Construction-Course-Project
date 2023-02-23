
/* definitions */

%{
  #include<stdio.h>
  #include<tokendef.h> 
%}

%%

 /* rules */

int { return KWD_INT; }
intg { return ID; }
main { return ID; }

. {return ERROR;}
%%

// user functions 
