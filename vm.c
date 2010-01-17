#include <stdlib.h>
#include "main.h"
#include "vm.h"
#include "y.tab.h"

void stack_print()
{
	printf("[");
	for (int i = 0; i < 10; ++i)
		printf("%i,", stack[i] != NULL ? stack[i]->ival : -1);
	printf("] : %i\n", stack_ptr);
}

void push(node* node)
{
	stack[stack_ptr++] = node;
}

node* pop()
{
	if (stack_ptr > 0)
		return stack[--stack_ptr];
	else
		return nil();
}

node* car(node* node)
{
	if (node == NULL)
		return nil();
	else
		return node->opr.op[0];
}

node* cdr(node* node)
{
	if (node == NULL)
		return nil();
	else if (node->opr.nops > 0 && node->opr.op[1] != NULL)
	{
		return node->opr.op[1];
	}
	else
		return nil();
}

node* cons(node* list, node* n)
{
	if (list->type == t_nil)
		return opr(LIST, 2, n, NULL);
	else if (n->type == t_nil)
		return opr(LIST, 2, list, NULL);
	else
		return opr(LIST, 2, n, list);
}

void display(node* node)
{
	if (node == NULL)
			return;
	
	switch (node->type)
	{
		case t_int:
			printf("%i\n", node->ival);
			break;
		case t_float:
			printf("%g\n", node->fval);
			break;
		case t_nil:
			printf("nil\n");
			break;
		case t_bool:
			printf("%s\n", node->ival > 0 ? "true" : "false");
			break;
		case t_cons:
			display(node->opr.op[0]);
			if (node->opr.nops > 1 && node->opr.op[1] != NULL)
			{
				printf(",");
				display(node->opr.op[1]);
			}
			break;
		case t_symbol:
			printf("%s\n", node->sval);
			break;				
	}
}

int length(node* node)
{
	if (node == NULL)
		return -1;
		
	if (node->type == t_cons)
	{
		if (node->opr.nops > 1 && node->opr.op[1] != NULL)
			return 1 + length(node->opr.op[1]);
		else
			return node->opr.nops;
	}
	else
		return 1;
}

node* add(node* x, node* y)
{
		if (x->type == t_float)
		{
				if (y->type == t_float)
						return fpval(x->fval + y->fval);
				else
						return fpval(x->fval + y->ival);
		}
		else
		{
				if (y->type == t_float)
						return fpval(x->ival + y->fval);
				else
						return con(x->ival + y->ival);
		}		
}

node* sub(node* x, node* y)
{
		if (x->type == t_float)
		{
				if (y->type == t_float)
						return fpval(x->fval - y->fval);
				else
						return fpval(x->fval - y->ival);
		}
		else
		{
				if (y->type == t_float)
						return fpval(x->ival - y->fval);
				else
						return con(x->ival - y->ival);
		}		
}

node* mul(node* x, node* y)
{
		if (x->type == t_float)
		{
				if (y->type == t_float)
						return fpval(x->fval * y->fval);
				else
						return fpval(x->fval * y->ival);
		}
		else
		{
				if (y->type == t_float)
						return fpval(x->ival * y->fval);
				else
						return con(x->ival * y->ival);
		}		
}

node* dvd(node* x, node* y)
{
	if (x->type == t_float)
	{
		if (y->type == t_float)
			return fpval(x->fval / y->fval);
		else
			return fpval(x->fval / y->ival);
	}
	else
	{
		if (y->type == t_float)
			return fpval(x->ival / y->fval);
		else
			return con(x->ival / y->ival);
	}		
}

node* lt(node* x, node* y)
{
		if (x->type == t_float)
		{
				if (y->type == t_float)
					return boolval(x->fval < y->fval);
				else
					return boolval(x->fval < y->ival);
		}
		else
		{
				if (y->type == t_float)
					return boolval(x->ival < y->fval);
				else
					return boolval(x->ival < y->ival);
		}		
}

node* gt(node* x, node* y)
{
		if (x->type == t_float)
		{
				if (y->type == t_float)
						return boolval(x->fval > y->fval);
				else
						return boolval(x->fval > y->ival);
		}
		else
		{
				if (y->type == t_float)
						return boolval(x->ival > y->fval);
				else
						return boolval(x->ival > y->ival);
		}		
}

node* lte(node* x, node* y)
{
		if (x->type == t_float)
		{
				if (y->type == t_float)
						return boolval(x->fval <= y->fval);
				else
						return boolval(x->fval <= y->ival);
		}
		else
		{
				if (y->type == t_float)
						return boolval(x->ival <= y->fval);
				else
						return boolval(x->ival <= y->ival);
		}		
}

node* gte(node* x, node* y)
{
		if (x->type == t_float)
		{
				if (y->type == t_float)
						return boolval(x->fval >= y->fval);
				else
						return boolval(x->fval >= y->ival);
		}
		else
		{
				if (y->type == t_float)
						return boolval(x->ival >= y->fval);
				else
						return boolval(x->ival >= y->ival);
		}		
}

node* list_eq(node* l1, node* l2)
{
	if (l1 == NULL && l2 == NULL)
		return boolval(true);

	if (eq(l1->opr.op[0], l2->opr.op[0])->ival = 1)
	{
		if (l1->opr.nops == 2 && l2->opr.nops == 2)
			return eq(l1->opr.op[1], l2->opr.op[1]);
		else
			return boolval(false);
	}
	else
		return boolval(false);
}

node* eq(node* x, node* y)
{
	if (x->type != y->type)
		return boolval(false);
		
	if (x->type == t_int || x->type == t_bool)
		return boolval(x->ival == y->ival);
		
	if (x->type == t_float)
		return boolval(x->fval == y->fval);
	
	if (x->type == t_nil)
		return boolval(true);
	
	if (x->type == t_cons)
		return list_eq(x, y);
	
	return boolval(false);
}

node* neq(node* x, node* y)
{
	if (x->type != y->type)
		return boolval(true);
		
	if (x->type == t_int || x->type == t_bool)
		return boolval(x->ival != y->ival);
		
	if (x->type == t_float)
		return boolval(x->fval != y->fval);
		
	if (x->type == t_nil || y->type == t_nil)
		return boolval(false);
	
	return boolval(false);
}

node* and(node* x, node* y)
{
	if (x->type == t_bool && y->type == t_bool)
	{
		return boolval(x->ival && y->ival);
	}
	else
	{
		// TODO probably should throw error
		return boolval(false);
	}
}

node* or(node* x, node* y)
{
	if (x->type == t_bool && y->type == t_bool)
	{
		return boolval(x->ival || y->ival);
	}
	else
	{
		// TODO probable should throw error
		return boolval(false);
	}
}

node* mod(node* x, node* y)
{
	if (x->type == t_int && y->type == t_int)
	{
		return con(x->ival % y->ival);
	}
	else
	{
		// TODO probable should throw error
		return con(-1);
	}
}
