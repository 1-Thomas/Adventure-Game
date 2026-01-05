bool readIntArg(int &out)
{
    if (!(std::cin >> out))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}