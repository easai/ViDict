#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QFont>

class Config : public QObject
{
public:
    explicit Config(QObject *parent = nullptr);
    Config(const Config &);
    Config &operator=(const Config &);

    QString format() const;

    void setFormat(const QString &newFormat);

    void setFont(const QFont &newFont);

    QFont font() const;

private:
    QFont m_font;
    QString m_format;
};

#endif // CONFIG_H
