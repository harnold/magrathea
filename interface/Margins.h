#ifndef interface_Margins_h
#define interface_Margins_h

//---------------------------------------------------------------------------

struct Margins
{
    int left, top, right, bottom;

    Margins() {}

    Margins(int left, int top, int right, int bottom)
        : left(left), top(top), right(right), bottom(bottom) {}

    bool IsNull() const
    { return left == 0 && top == 0 && right == 0 && bottom == 0; }
};

//---------------------------------------------------------------------------

#endif
