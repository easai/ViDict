#include "config.h"

Config::Config(QObject *parent) : QObject{parent} {}

Config::Config(const Config &other) : m_format(other.m_format), m_font(other.m_font) {}

Config &Config::operator=(const Config &other) {
  if (this != &other) {
    m_format = other.m_format;
      m_font=other.m_font;
  }
  return *this;
}

QString Config::format() const { return m_format; }

void Config::setFormat(const QString &newFormat) { m_format = newFormat; }

void Config::setFont(const QFont &newFont) { m_font = newFont; }

QFont Config::font() const { return m_font; }
