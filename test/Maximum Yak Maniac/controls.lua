require 'button'

controls = {
    -- Player one.
    {
        up = Button(plum.key.W);
        left = Button(plum.key.A);
        eat = Button(plum.key.S);
        right = Button(plum.key.D);
    };
    -- Player two.
    {
        up = Button(plum.key.Up);
        left = Button(plum.key.Left);
        eat = Button(plum.key.Down);
        right = Button(plum.key.Right);
    };
};