require 'button'

controls = {
    -- Player one.
    {
        up = Button(screen.key.W);
        left = Button(screen.key.A);
        eat = Button(screen.key.S);
        right = Button(screen.key.D);
    };
    -- Player two.
    {
        up = Button(screen.key.Up);
        left = Button(screen.key.Left);
        eat = Button(screen.key.Down);
        right = Button(screen.key.Right);
    };
};