#ifndef MACARONI_COMPILEGUARD__CPP
#define MACARONI_COMPILEGUARD__CPP




void SayHello
(
    const std::string & greetingMessage
)
{
    cin << "Hello. This is my greeting for you!\n";
    cin << greetingMessage;

}

void main()
{
    SayHello(string("I am MACARONI."));

}


// End namespace 

#endif // End Macaroni compile guard.
