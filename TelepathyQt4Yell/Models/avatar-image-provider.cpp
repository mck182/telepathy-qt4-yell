/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2010 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <TelepathyQt4Yell/Models/AvatarImageProvider>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>

namespace Tp
{

struct TELEPATHY_QT4_MODELS_NO_EXPORT AvatarImageProvider::Private
{
    Private(const Tp::AccountManagerPtr &am)
        : mAM(am)
    {
    }

    Tp::AccountManagerPtr mAM;
};

AvatarImageProvider::AvatarImageProvider(const AccountManagerPtr &am)
    : QDeclarativeImageProvider(Image),
      mPriv(new Private(am))
{
}

AvatarImageProvider::~AvatarImageProvider()
{
    delete mPriv;
}

QString AvatarImageProvider::urlFor(const AccountPtr &account)
{
    return QString::fromLatin1("image://avatars/") + account->uniqueIdentifier();
}

void AvatarImageProvider::registerProvider(QDeclarativeEngine *engine, const AccountManagerPtr &am)
{
    engine->addImageProvider(QString::fromLatin1("avatars"), new AvatarImageProvider(am));
}

QImage AvatarImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QString path = QString::fromLatin1(TELEPATHY_ACCOUNT_OBJECT_PATH_BASE "/") + id;
    AccountPtr account = mPriv->mAM->accountForPath(path);
    QImage image;
    image.loadFromData(account->avatar().avatarData);
    if (size) {
        *size = image.size();
    }
    return image;
}

}
