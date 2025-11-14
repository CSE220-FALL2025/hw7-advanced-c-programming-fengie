// feng cheng li, 115607859
#include "hw7.h"

bst_sf *insert_bst_sf(matrix_sf *mat, bst_sf *root)
{
    // if matric is null, nothing to insert
    if (mat == NULL)
        return root;

    // if root is null, just make bst node with mat and put it there
    if (root == NULL)
    {
        bst_sf *res = malloc(sizeof(bst_sf));
        if (!res)
            return NULL;

        res->mat = mat;
        res->right_child = NULL;
        res->left_child = NULL;

        return res;
    }

    // otherwise, if mat is less than node, then recursively run it left.
    // if it is greater, run it rigt.
    // if it is the same, replace it.
    if (root->mat->name > mat->name)
        root->left_child = insert_bst_sf(mat, root->left_child);
    else if (root->mat->name < mat->name)
        root->right_child = insert_bst_sf(mat, root->right_child);
    else
    {
        free(root->mat);
        root->mat = mat;
    }

    return root;
}

matrix_sf *find_bst_sf(char name, bst_sf *root)
{
    // if root is null, then just return null.
    if (root == NULL)
        return NULL;

    // if name is less, go left recursively. If greater, go right.
    // otherwise, they are same so return node.
    if (name > root->mat->name)
        return find_bst_sf(name, root->right_child);
    else if (name < root->mat->name)
        return find_bst_sf(name, root->left_child);
    else
        return root->mat;
}

void free_bst_sf(bst_sf *root)
{

    // base case
    if (root == NULL)
        return;

    // free left and right
    free_bst_sf(root->left_child);
    free_bst_sf(root->right_child);

    // free the matrices and root.
    free(root->mat);
    free(root);
}

void free_bst_except_one(bst_sf *root, matrix_sf *keep)
{
    // base case
    if (root == NULL)
        return;

    // free left and right
    free_bst_except_one(root->left_child, keep);
    free_bst_except_one(root->right_child, keep);

    // free the matrices and root.

    if (root->mat != keep)
        free(root->mat);

    free(root);
}

matrix_sf *add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2)
{

    // checking error cases
    if (!mat1 || !mat2)
        return NULL;
    if (!(mat1->num_cols == mat2->num_cols) || !(mat1->num_rows == mat2->num_rows))
        return NULL;

    // allocate memory
    size_t elem_num = mat1->num_rows * mat2->num_cols;
    matrix_sf *res = malloc(sizeof(matrix_sf) + elem_num * sizeof(int));

    if (!res)
        return NULL;

    // setting data
    res->name = '+';
    res->num_rows = mat1->num_rows;
    res->num_cols = mat2->num_cols;

    // do addition
    for (int i = 0; i < elem_num; i++)
        res->values[i] = mat1->values[i] + mat2->values[i];

    return res;
}

matrix_sf *mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2)
{

    // checking error cases
    if (!mat1 || !mat2)
        return NULL;
    if (mat1->num_cols != mat2->num_rows)
        return NULL;

    // allocate memory
    size_t elem_num = mat1->num_rows * mat2->num_cols;
    matrix_sf *res = malloc(sizeof(matrix_sf) + elem_num * sizeof(int));

    if (!res)
        return NULL;

    // setting data
    res->name = '*';
    res->num_rows = mat1->num_rows;
    res->num_cols = mat2->num_cols;

    // initialize res matrix values to 0;
    for (int i = 0; i < elem_num; i++)
        res->values[i] = 0;

    // multiplication
    for (int i = 0; i < mat1->num_rows; i++)
    {
        for (int j = 0; j < mat1->num_cols; j++)
        {
            for (int k = 0; k < mat2->num_cols; k++)
            {
                res->values[i * mat2->num_cols + k] +=
                    mat1->values[i * mat1->num_cols + j] * mat2->values[j * mat2->num_cols + k];
            }
        }
    }

    return res;
}

matrix_sf *transpose_mat_sf(const matrix_sf *mat)
{
    // checking error
    if (!mat)
        return NULL;

    // allocate memory for res
    matrix_sf *res = malloc(sizeof(matrix_sf) +
                            mat->num_cols * mat->num_rows * sizeof(int));

    if (!res)
        return NULL;

    // setting data
    res->name = '&';
    res->num_rows = mat->num_cols;
    res->num_cols = mat->num_rows;

    // doing transpose
    for (int i = 0; i < mat->num_rows; i++)
        for (int j = 0; j < mat->num_cols; j++)
            res->values[j * res->num_cols + i] = mat->values[i * mat->num_cols + j];

    return res;
}

matrix_sf *create_matrix_sf(char name, const char *expr)
{

    if (!expr)
        return NULL;

    unsigned int num_rows = 0, num_cols = 0;
    int count = 0;
    char *ptr = expr;

    // skip white space in front
    while (*ptr && isspace(*ptr))
    {
        ptr++;
    }

    // get numrow
    while (*ptr && *ptr != '[')
    {
        char *endptr = NULL;
        num_rows = (unsigned int)strtol(ptr, &endptr, 10);

        // check if ptr actually moved
        if (ptr == endptr)
            return NULL;

        ptr = endptr;
        break;
    }

    // check numrows eror
    if (num_rows == 0)
    {
        return NULL;
    }

    // skip whitesapce
    while (*ptr && isspace(*ptr))
    {
        ptr++;
    }

    // get numcol
    while (*ptr && *ptr != '[')
    {
        char *endptr = NULL;
        num_cols = (unsigned int)strtol(ptr, &endptr, 10);

        if (ptr == endptr)
            return NULL;
        ptr = endptr;
        break;
    }

    if (num_cols == 0)
        return NULL;

    // skip whitespace
    while (*ptr && isspace(*ptr))
    {
        ptr++;
    }

    // move ptr to [
    while (*ptr && *ptr != '[')
    {
        ptr++;
    }

    // error check for ptr
    if (*ptr != '[')
        return NULL;

    // move it to one after [
    ptr++;

    // allocate memory and data
    matrix_sf *res = malloc(sizeof(matrix_sf) + ((size_t)num_cols * (size_t)num_rows * sizeof(int)));

    if (!res)
        return NULL;

    res->name = name;
    res->num_cols = num_cols;
    res->num_rows = num_rows;

    int valuesIndex = 0;

    // get values
    while (*ptr && *ptr != ']')
    {

        // skip whitespaces and other stuff to get values
        while (*ptr && *ptr != ']' && !(*ptr == '-' || (*ptr >= '0' && *ptr <= '9')))
        {
            ptr++;
        }

        // checking if it reaches end, break ealry.
        if (*ptr == ']' || !*ptr)
            break;

        // check in case overflow
        if (valuesIndex >= num_cols * num_rows)
        {
            free(res);
            return NULL;
        }

        // get the value, put it into values at the index, then advance the index.
        char *endptr = NULL;
        res->values[valuesIndex] = strtol(ptr, &endptr, 10);
        valuesIndex++;

        if (ptr == endptr)
        {

            free(res);
            return NULL;
        }

        ptr = endptr;
    }

    // check for incorrect dimensions or wrong amount of values.

    if (valuesIndex != num_rows * num_cols)
    {
        free(res);
        return NULL;
    }

    return res;
}

// helper to get precidence
int prec(char operation)
{
    if (operation == '\'')
        return 3;
    if (operation == '*')
        return 2;
    if (operation == '+')
        return 1;
    return -1;
}

// make a stack
typedef struct
{
    char *data;
    int top;
    // int cap;
} stack;

// methods for stack
void free_stack(stack *s)
{
    if (!s)
        return;
    free(s->data);
    free(s);
}
int empty(stack *s)
{
    return s->top < 0;
}
char peek(stack *s)
{
    return *(s->data + s->top);
}
void push(stack *s, char c)
{
    *(s->data + ++(s->top)) = c;
}
char pop(stack *s)
{
    return *(s->data + s->top--);
}

char *infix2postfix_sf(char *infix)
{
    // allocating space for res
    int n = strlen(infix);
    char *res = malloc(n + 1);
    if (!res)
    {
        return NULL;
    }

    // initilaziting stack
    stack *tempStack = malloc(sizeof(*tempStack));
    if (!tempStack)
    {
        free(res);
        return NULL;
    }

    tempStack->data = malloc(n);
    // tempStack->cap = n;
    tempStack->top = -1;

    if (!tempStack->data)
    {
        free_stack(tempStack);
        free(res);
        return NULL;
    }

    int resLength = 0;

    // going through infix
    for (int i = 0; i < n; i++)
    {

        char c = *(infix + i);

        // checking if it is white space
        if (c == ' ' || c == '\t' || c == '\n')
            continue;

        // checking if scannedd char is operand.
        // if it is, add to res.
        if (
            c >= 'A' && c <= 'Z' ||
            c >= 'a' && c <= 'z')
        {

            *(res + resLength) = c;
            resLength++;

            // check for consecutive ' and just add it on the end
            // make sure to not go out of bounds
            while (i + 1 < n && *(infix + 1 + i) == '\'')
            {
                *(res + resLength) = '\'';
                resLength++;
                i++;
            }
        }

        // checking for '('. if it is, just push it to stack
        else if (c == '(')
            push(tempStack, c);

        // ceck for ')'.
        // if it is, pop and to output string from stack until find '('
        else if (c == ')')
        {
            while (!empty(tempStack) && peek(tempStack) != '(')
            {
                *(res + resLength) = peek(tempStack);
                resLength++;
                pop(tempStack);
            }

            // if no '(' just free memory and return null.
            if (empty(tempStack))
            {
                free_stack(tempStack);
                free(res);
                return NULL;
            }

            // get rid of ( in stack
            pop(tempStack);
        }

        // if operator is scanned.
        else if (c == '+' || c == '*')
        {

            // stack is not empty, precidence check, and there is no open bracket
            while (!empty(tempStack) && peek(tempStack) != '(' && prec(c) <= prec(peek(tempStack)))
            {
                *(res + resLength++) = peek(tempStack);
                pop(tempStack);
            }
            push(tempStack, c);
        }

        // lone transpose is scanned
        else if (c == '\'')
        {
            *(res + resLength++) = '\'';
        }

        // invalid input
        else
        {
            free_stack(tempStack);
            free(res);
            return NULL;
        }
    }
    // pop remaining elements from stack.
    while (!empty(tempStack))
    {
        // also check for ( without )
        if (peek(tempStack) == '(')
        {
            free_stack(tempStack);
            free(res);
            return NULL;
        }

        *(res + resLength++) = pop(tempStack);
    }

    // terminate res string and free stack.
    *(res + resLength) = '\0';
    free_stack(tempStack);

    return res;
}

// make a m_stack
typedef struct
{
    matrix_sf **data;
    int top;
    // int cap;
} m_stack;

// methods for m_stack
void mfree_stack(m_stack *s)
{
    if (!s)
        return;
    free(s->data);
    free(s);
}
int mempty(m_stack *s)
{
    return s->top < 0;
}
matrix_sf *mpeek(m_stack *s)
{
    return *(s->data + s->top);
}
void mpush(m_stack *s, matrix_sf *c)
{
    *(s->data + ++(s->top)) = c;
}
matrix_sf *mpop(m_stack *s)
{
    return *(s->data + s->top--);
}

matrix_sf *evaluate_expr_sf(char name, char *expr, bst_sf *root)
{
    // use infix 2 postfix function
    char *postfix = infix2postfix_sf(expr);

    if (!postfix)
        return NULL;

    // initialize empty stack
    m_stack *st = malloc(sizeof(*st));
    if (!st)
        return NULL;

    st->data = NULL;
    st->top = -1;

    st->data = malloc(sizeof(matrix_sf *) * 100);
    if (!st->data)
    {
        mfree_stack(st);
        return NULL;
    }

    int n = strlen(postfix);

    // loop thru postfix string
    for (int i = 0; i < n; i++)
    {
        char c = *(postfix + i);

        // check for name. push the matrix in stack
        if (
            c >= 'A' && c <= 'Z' ||
            c >= 'a' && c <= 'z')
        {
            matrix_sf *ptr = find_bst_sf(c, root);

            mpush(st, ptr);
        }

        // if ', then make trans, pop matrix, then push it back in stack
        else if (c == '\'')
        {

            // getting poped matrix
            matrix_sf *op = mpop(st);
            if (!op)
            {
                free(postfix);
                mfree_stack(st);
                return NULL;
            }

            // transposing it
            matrix_sf *trans = transpose_mat_sf(op);
            if (!trans)
            {
                free(postfix);
                mfree_stack(st);
                return NULL;
            }

            // checking if the name is alphaetical.
            // if not, free it.
            int opAlpha = 0;
            if (op->name >= 'A' && op->name <= 'Z' ||
                op->name >= 'a' && op->name <= 'z')
            {
                opAlpha = 1;
            }

            if (opAlpha == 0)
            {
                free(op);
            }

            mpush(st, trans);
        }
        else if (c == '*')
        {
            // pop to get operandns.
            matrix_sf *temp1 = mpop(st);
            matrix_sf *temp2 = mpop(st);

            // check if they exist
            if (!temp1 || !temp2)
            {
                free(postfix);
                mfree_stack(st);
                return NULL;
            }

            // get multi matrix
            matrix_sf *mult = mult_mats_sf(temp2, temp1);

            if (!mult)
            {
                free(postfix);
                mfree_stack(st);
                return NULL;
            }

            int temp1_alpha = 0;
            int temp2_alpha = 0;

            // check if theyre alphabebt.
            if (temp1->name >= 'A' && temp1->name <= 'Z' ||
                temp1->name >= 'a' && temp1->name <= 'z')
            {
                temp1_alpha = 1;
            }
            if (temp2->name >= 'A' && temp2->name <= 'Z' ||
                temp2->name >= 'a' && temp2->name <= 'z')
            {
                temp2_alpha = 1;
            }

            // free if theyre not
            if (!temp1_alpha)
                free(temp1);
            if (!temp2_alpha)
                free(temp2);

            // set the name and push multipliced matrix
            // mult->name = name;
            mpush(st, mult);
        }

        // logic same as multiply but its addition.
        else if (c == '+')
        {
            // pop to get operandns.
            matrix_sf *temp1 = mpop(st);
            matrix_sf *temp2 = mpop(st);

            // check if they exist
            if (!temp1 || !temp2)
            {
                mfree_stack(st);
                free(postfix);
                return NULL;
            }

            // get add matrix
            matrix_sf *add = add_mats_sf(temp1, temp2);

            if (!add)
            {
                mfree_stack(st);
                free(postfix);
                return NULL;
            }

            int temp1_alpha = 0;
            int temp2_alpha = 0;

            // check if theyre alphabebt.
            if (temp1->name >= 'A' && temp1->name <= 'Z' ||
                temp1->name >= 'a' && temp1->name <= 'z')
            {
                temp1_alpha = 1;
            }
            if (temp2->name >= 'A' && temp2->name <= 'Z' ||
                temp2->name >= 'a' && temp2->name <= 'z')
            {
                temp2_alpha = 1;
            }

            if (!temp1_alpha)
                free(temp1);
            if (!temp2_alpha)
                free(temp2);

            // add->name = name;
            mpush(st, add);
        }
    }

    // if st is empty, there is no res
    if (mempty(st))
    {
        mfree_stack(st);
        free(postfix);
        return NULL;
    }
    // capture res
    matrix_sf *res = mpop(st);

    // clearing leftovers in stack
    while (!mempty(st))
    {
        matrix_sf *temp = mpop(st);

        int temp_alpha = 0;

        // freeing nonalphabet names
        if (temp->name >= 'A' && temp->name <= 'Z' ||
            temp->name >= 'a' && temp->name <= 'z')
        {
            temp_alpha = 1;
        }

        if (temp_alpha == 0)
            free(temp);
    }

    // freeing stack memory and postfix

    mfree_stack(st);
    free(postfix);

    // set res name
    if (res)
        res->name = name;

    return res;
}

matrix_sf *execute_script_sf(char *filename)
{

    // open file for reaad
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return NULL;

    // initialize str,max line size, and lien length
    char *str = NULL;
    size_t max_line_size = MAX_LINE_LEN;
    ssize_t line_length;

    // initilaize bst root and resulting matrix
    bst_sf *root = NULL;
    matrix_sf *res;

    // read file per line
    while ((line_length = getline(&str, &max_line_size, fp)) != -1)
    {

        // remove new lines at the end
        while (line_length > 0 && (*(str + line_length - 1) == '\n' || *(str + line_length - 1) == '\r'))
        {
            *(str + --line_length) = '\0';
        }

        // skip until yoou see non whitespace
        // p should also be pointer to name of new matrix
        char *p = str;
        while (*p && isspace(*p))
        {
            p++;
        }

        // skip linens with nothing
        if (*p == '\0')
            continue;
        // skip lines with non alphabet matrix names
        if (!(*p >= 'a' && *p <= 'z') && !(*p >= 'A' && *p <= 'Z'))
        {
            continue;
        }

        // put a pointer at the equal sign
        char *equal = p;
        while (*equal)
        {
            if (*equal == '=')
                break;
            equal++;
        }

        // skip liens with no equal sign.
        if (*equal == '\0')
            continue;

        // put pointer at expression after equal sign, and skip white space.
        char *expression = equal + 1;

        while (*expression)
        {
            if (isspace(*expression))
                expression++;
            else
                break;
        }

        // now expression is at beginning of expression.

        // if it is at \0, then we just skip the line becuse there is no expression
        if (*expression == '\0')
            continue;

        matrix_sf *mat;

        // check if therer is a '['. if it is, that means that contains a create matrix thing.
        char *checkForMat = expression;

        while (*checkForMat)
        {
            if (*checkForMat == '[')
                break;
            checkForMat++;
        }

        // if it is a matrix setting
        if (*checkForMat == '[')
        {
            mat = create_matrix_sf(*p, expression);
        }

        // if there is no bracket, it is a matrix operation line
        else
        {
            mat = evaluate_expr_sf(*p, expression, root);
        }

        // error check
        if (!mat)
        {
            free(str);
            fclose(fp);

            return NULL;
        }

        // insert matrix to bst.
        root = insert_bst_sf(mat, root);
        if (!root)
        {
            free(str);
            fclose(fp);

            return NULL;
        }

        // this will happen every loop. last loop will result in res.
        res = mat;
    }

    free(str);
    fclose(fp);

    // freeing bst except res so we can return it
    free_bst_except_one(root, res);
    // free_bst_sf(root);

    return res;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[])
{
    matrix_sf *m = malloc(sizeof(matrix_sf) + num_rows * num_cols * sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows * num_cols * sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
void print_matrix_sf(matrix_sf *mat)
{
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d ", mat->num_rows, mat->num_cols);
    for (unsigned int i = 0; i < mat->num_rows * mat->num_cols; i++)
    {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows * mat->num_cols - 1)
            printf(" ");
    }
    printf("\n");
}
