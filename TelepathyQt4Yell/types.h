/*
 * This file is part of TelepathyQt4Yell
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

#ifndef _TelepathyQt4Yell_types_h_HEADER_GUARD_
#define _TelepathyQt4Yell_types_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_YELL_HEADER
#error IN_TELEPATHY_QT4_YELL_HEADER
#endif

#include <TelepathyQt4Yell/_gen/types.h>

#include <TelepathyQt4/Types>

namespace Tpy
{

TELEPATHY_QT4_YELL_EXPORT void registerTypes();

class CallChannel;
class CallContent;
class CallStream;

typedef Tp::SharedPtr<CallChannel> CallChannelPtr;
typedef Tp::SharedPtr<CallContent> CallContentPtr;
typedef Tp::SharedPtr<CallStream> CallStreamPtr;

} // Tpy

#endif
