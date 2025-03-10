/*
    SPDX-FileCopyrightText: 2018 Marco Martin <notmart@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#ifndef KWAYLAND_CLIENT_LINGMOVIRTUALDESKTOP_H
#define KWAYLAND_CLIENT_LINGMOVIRTUALDESKTOP_H

#include <QObject>

#include "KWayland/Client/kwaylandclient_export.h"

struct org_kde_lingmo_virtual_desktop_management;
struct org_kde_lingmo_virtual_desktop;

namespace KWayland
{
namespace Client
{
class EventQueue;
class LingmoVirtualDesktop;

/**
 * @short Wrapper for the org_kde_lingmo_virtual_desktop_management interface.
 *
 * This class provides a convenient wrapper for the org_kde_lingmo_virtual_desktop_management interface.
 *
 * To use this class one needs to interact with the Registry. There are two
 * possible ways to create the LingmoVirtualDesktopManagement interface:
 * @code
 * LingmoVirtualDesktopManagement *c = registry->createLingmoVirtualDesktopManagement(name, version);
 * @endcode
 *
 * This creates the LingmoVirtualDesktopManagement and sets it up directly. As an alternative this
 * can also be done in a more low level way:
 * @code
 * LingmoVirtualDesktopManagement *c = new LingmoVirtualDesktopManagement;
 * c->setup(registry->bindLingmoVirtualDesktopManagement(name, version));
 * @endcode
 *
 * The LingmoVirtualDesktopManagement can be used as a drop-in replacement for any org_kde_lingmo_virtual_desktop_management
 * pointer as it provides matching cast operators.
 * @since 5.52
 *
 * @see Registry
 **/
class KWAYLANDCLIENT_EXPORT LingmoVirtualDesktopManagement : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a new LingmoVirtualDesktopManagement.
     * Note: after constructing the LingmoVirtualDesktopManagement it is not yet valid and one needs
     * to call setup. In order to get a ready to use LingmoVirtualDesktopManagement prefer using
     * Registry::createLingmoVirtualDesktopManagement.
     **/
    explicit LingmoVirtualDesktopManagement(QObject *parent = nullptr);
    ~LingmoVirtualDesktopManagement() override;

    /**
     * Setup this LingmoVirtualDesktopManagement to manage the @p lingmovirtualdesktopmanagement.
     * When using Registry::createLingmoVirtualDesktopManagement there is no need to call this
     * method.
     **/
    void setup(org_kde_lingmo_virtual_desktop_management *lingmovirtualdesktopmanagement);
    /**
     * @returns @c true if managing a org_kde_lingmo_virtual_desktop_management.
     **/
    bool isValid() const;
    /**
     * Releases the org_kde_lingmo_virtual_desktop_management interface.
     * After the interface has been released the LingmoVirtualDesktopManagement instance is no
     * longer valid and can be setup with another org_kde_lingmo_virtual_desktop_management interface.
     **/
    void release();
    /**
     * Destroys the data held by this LingmoVirtualDesktopManagement.
     * This method is supposed to be used when the connection to the Wayland
     * server goes away. If the connection is not valid anymore, it's not
     * possible to call release anymore as that calls into the Wayland
     * connection and the call would fail. This method cleans up the data, so
     * that the instance can be deleted or set up to a new org_kde_lingmo_virtual_desktop_management interface
     * once there is a new connection available.
     *
     * It is suggested to connect this method to ConnectionThread::connectionDied:
     * @code
     * connect(connection, &ConnectionThread::connectionDied, lingmovirtualdesktopmanagement, &LingmoVirtualDesktopManagement::destroy);
     * @endcode
     *
     * @see release
     **/
    void destroy();

    /**
     * Sets the @p queue to use for creating objects with this LingmoVirtualDesktopManagement.
     **/
    void setEventQueue(EventQueue *queue);

    /**
     * @returns The event queue to use for creating objects with this LingmoVirtualDesktopManagement.
     * The object is owned by the manager and the caller should not delete it.
     **/
    EventQueue *eventQueue();

    /**
     * @returns the LingmoVirtualDesktop representing the desktop id.
     * The LingmoVirtualDesktop instance is guaranteed to be unique for each id.
     */
    LingmoVirtualDesktop *getVirtualDesktop(const QString &id);

    /**
     * Requests for the desktop identified by id to be removed.
     * The server may or may not acconsent to the request.
     */
    void requestRemoveVirtualDesktop(const QString &id);

    /**
     * Ask the server to create a new virtual desktop, and position it at a specified position.
     * If the position is zero or less, it will be positioned at the beginning,
     * if the cosition is the count or more, it will be positioned at the end.
     * @param name The name we want for the desktop
     * @param position The position for the desktop to be created
     */
    void requestCreateVirtualDesktop(const QString &name, quint32 position = std::numeric_limits<uint32_t>::max());

    /**
     * @returns All the existent virtual desktops
     */
    QList<LingmoVirtualDesktop *> desktops() const;

    /**
     * @returns How many rows the virtual desktops should be laid out into
     * @since 5.55
     */
    quint32 rows() const;

    operator org_kde_lingmo_virtual_desktop_management *();
    operator org_kde_lingmo_virtual_desktop_management *() const;

Q_SIGNALS:
    void removed();

    /**
     * Emitted when a new desktop has been added
     */
    void desktopCreated(const QString &id, quint32 position);

    /**
     * Emitted when a desktop has been removed
     */
    void desktopRemoved(const QString &id);

    /**
     * Emitted when the number of rows of virtual desktops has been changed by the server
     * @since 5.55
     */
    void rowsChanged(quint32 rows);

    /**
     * This event is sent after all other properties has been
     * sent after binding to the desktop manager object and after any
     * other property changes done after that. This allows
     * changes to the org_kde_lingmo_virtual_desktop_management properties
     * to be seen as atomic, even if they happen via multiple events.
     */
    void done();

private:
    class Private;
    QScopedPointer<Private> d;
};

class KWAYLANDCLIENT_EXPORT LingmoVirtualDesktop : public QObject
{
    Q_OBJECT
public:
    ~LingmoVirtualDesktop() override;

    /**
     * Setup this LingmoVirtualDesktop to manage the @p lingmovirtualdesktop.
     * When using LingmoVirtualDesktopManagement::createLingmoVirtualDesktop there is no need to call this
     * method.
     **/
    void setup(org_kde_lingmo_virtual_desktop *lingmovirtualdesktop);

    /**
     * @returns @c true if managing a org_kde_lingmo_virtual_desktop.
     **/
    bool isValid() const;

    /**
     * Releases the org_kde_lingmo_virtual_desktop interface.
     * After the interface has been released the LingmoVirtualDesktop instance is no
     * longer valid and can be setup with another org_kde_lingmo_virtual_desktop interface.
     **/
    void release();

    /**
     * Destroys the data held by this LingmoVirtualDesktop.
     * This method is supposed to be used when the connection to the Wayland
     * server goes away. If the connection is not valid anymore, it's not
     * possible to call release anymore as that calls into the Wayland
     * connection and the call would fail. This method cleans up the data, so
     * that the instance can be deleted or set up to a new org_kde_lingmo_virtual_desktop interface
     * once there is a new connection available.
     *
     * It is suggested to connect this method to ConnectionThread::connectionDied:
     * @code
     * connect(connection, &ConnectionThread::connectionDied, lingmovirtualdesktop, &LingmoVirtualDesktop::destroy);
     * @endcode
     *
     * @see release
     **/
    void destroy();

    /**
     * Requests this desktop to be activated.
     * The server may or may not decide to consent to the request.
     */
    void requestActivate();

    /**
     * @returns The unique id of this desktop. The format of the id is decided by the compositor
     */
    QString id() const;

    /**
     * @returns User readable name for the desktop.
     */
    QString name() const;

    /**
     * @returns True if the desktop is the active one.
     * when this property changes, activated or deactivated will be emitted.
     * @see activated
     * @see deactivated
     */
    bool isActive() const;

    operator org_kde_lingmo_virtual_desktop *();
    operator org_kde_lingmo_virtual_desktop *() const;

Q_SIGNALS:
    /**
     * TODO: activeChanged(bool)?
     * Emitted when this desktop has been activated by the server
     */
    void activated();

    /**
     * Emitted when this desktop has been activated by the server
     */
    void deactivated();

    /**
     * This event is sent after all other properties has been
     * sent after binding to the desktop manager object and after any
     * other property changes done after that. This allows
     * changes to the org_kde_lingmo_virtual_desktop properties
     * to be seen as atomic, even if they happen via multiple events.
     */
    void done();

    /**
     * This virtual desktop has just been removed by the server:
     * This object itself is about to be deleted. All windows will
     * lose the association to this desktop.
     */
    void removed();

private:
    friend class LingmoVirtualDesktopManagement;
    explicit LingmoVirtualDesktop(QObject *parent = nullptr);
    class Private;
    QScopedPointer<Private> d;
};

}
}

#endif
