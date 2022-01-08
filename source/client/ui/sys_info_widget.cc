//
// Aspia Project
// Copyright (C) 2021 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#include "client/ui/sys_info_widget.h"

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QLocale>
#include <QTreeWidgetItem>

namespace client {

namespace {

void copyTextToClipboard(const QString& text)
{
    if (text.isEmpty())
        return;

    QClipboard* clipboard = QApplication::clipboard();
    if (!clipboard)
        return;

    clipboard->setText(text);
}

} // namespace

SysInfoWidget::SysInfoWidget(QWidget* parent)
    : QWidget(parent)
{
    // Nothing
}

// static
QString SysInfoWidget::sizeToString(int64_t size)
{
    static const int64_t kKB = 1024LL;
    static const int64_t kMB = kKB * 1024LL;
    static const int64_t kGB = kMB * 1024LL;
    static const int64_t kTB = kGB * 1024LL;

    QString units;
    int64_t divider;

    if (size >= kTB)
    {
        units = tr("TB");
        divider = kTB;
    }
    else if (size >= kGB)
    {
        units = tr("GB");
        divider = kGB;
    }
    else if (size >= kMB)
    {
        units = tr("MB");
        divider = kMB;
    }
    else if (size >= kKB)
    {
        units = tr("kB");
        divider = kKB;
    }
    else
    {
        units = tr("B");
        divider = 1;
    }

    return QString("%1 %2")
        .arg(static_cast<double>(size) / static_cast<double>(divider), 0, 'g', 4)
        .arg(units);
}

QString SysInfoWidget::delayToString(uint64_t delay)
{
    uint64_t days = (delay / 86400);
    uint64_t hours = (delay % 86400) / 3600;
    uint64_t minutes = ((delay % 86400) % 3600) / 60;
    uint64_t seconds = ((delay % 86400) % 3600) % 60;

    QString seconds_string = tr("%n seconds", "", static_cast<int>(seconds));
    QString minutes_string = tr("%n minutes", "", static_cast<int>(minutes));
    QString hours_string = tr("%n hours", "", static_cast<int>(hours));

    if (!days)
    {
        if (!hours)
        {
            if (!minutes)
            {
                return seconds_string;
            }
            else
            {
                return minutes_string + QLatin1Char(' ') + seconds_string;
            }
        }
        else
        {
            return hours_string + QLatin1Char(' ') +
                   minutes_string + QLatin1Char(' ') +
                   seconds_string;
        }
    }
    else
    {
        QString days_string = tr("%n days", "", static_cast<int>(days));

        return days_string + QLatin1Char(' ') +
               hours_string + QLatin1Char(' ') +
               minutes_string + QLatin1Char(' ') +
               seconds_string;
    }
}

// static
QString SysInfoWidget::speedToString(uint64_t speed)
{
    static const uint64_t kKbps = 1000ULL;
    static const uint64_t kMbps = kKbps * 1000ULL;
    static const uint64_t kGbps = kMbps * 1000ULL;

    QString units;
    uint64_t divider;

    if (speed >= kGbps)
    {
        units = tr("Gbps");
        divider = kGbps;
    }
    else if (speed >= kMbps)
    {
        units = tr("Mbps");
        divider = kMbps;
    }
    else if (speed >= kKbps)
    {
        units = tr("Kbps");
        divider = kKbps;
    }
    else
    {
        units = tr("bps");
        divider = 1;
    }

    return QString("%1 %2")
        .arg(static_cast<double>(speed) / static_cast<double>(divider), 0, 'g', 4)
        .arg(units);
}

// static
QString SysInfoWidget::timeToString(time_t time)
{
    return QLocale::system().toString(QDateTime::fromSecsSinceEpoch(time), QLocale::ShortFormat);
}

void SysInfoWidget::copyRow(QTreeWidgetItem* item)
{
    if (!item)
        return;

    QString name = item->text(0);
    QString value = item->text(1);

    if (value.isEmpty())
        copyTextToClipboard(name);
    else
        copyTextToClipboard(name + QLatin1String(": ") + value);
}

void SysInfoWidget::copyColumn(QTreeWidgetItem* item, int column)
{
    if (!item)
        return;

    copyTextToClipboard(item->text(column));
}

} // namespace client