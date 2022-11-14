#ifndef CONFIG_H
#define CONFIG_H


class Config
{
public:
    Config();

    static void loadData();

    static void getProperty();

private:
    static bool loadedData;
};

#endif // CONFIG_H
