/*
    Copyright 2013 Lukas Tinkl <ltinkl@redhat.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "btwidget.h"
#include "ui_bt.h"
#include "uiutils.h"

#include <KLocalizedString>

#include <NetworkManagerQt/Utils>

BtWidget::BtWidget(const NetworkManager::Setting::Ptr &setting, QWidget* parent, Qt::WindowFlags f):
    SettingWidget(setting, parent, f),
    m_ui(new Ui::BtWidget)
{
    m_ui->setupUi(this);

    m_ui->type->addItem(i18n("DUN (dial up networking)"), NetworkManager::BluetoothSetting::Dun);
    m_ui->type->addItem(i18n("PAN (personal area network)"), NetworkManager::BluetoothSetting::Panu);

    m_ui->type->setEnabled(false);

    // Connect for setting check
    watchChangedSetting();

    // Connect for validity check
    connect(m_ui->bdaddr, &HwAddrComboBox::hwAddressChanged, this, &BtWidget::slotWidgetChanged);

    KAcceleratorManager::manage(this);

    if (setting) {
        loadConfig(setting);
    }
}

BtWidget::~BtWidget()
{
    delete m_ui;
}

void BtWidget::loadConfig(const NetworkManager::Setting::Ptr &setting)
{
    NetworkManager::BluetoothSetting::Ptr btSetting = setting.staticCast<NetworkManager::BluetoothSetting>();

    m_ui->bdaddr->init(NetworkManager::Device::Bluetooth, NetworkManager::macAddressAsString(btSetting->bluetoothAddress()));
    m_ui->type->setCurrentIndex(m_ui->type->findData(btSetting->profileType()));
}

QVariantMap BtWidget::setting() const
{
    NetworkManager::BluetoothSetting btSetting;

    btSetting.setBluetoothAddress(NetworkManager::macAddressFromString(m_ui->bdaddr->hwAddress()));
    btSetting.setProfileType(static_cast<NetworkManager::BluetoothSetting::ProfileType>(m_ui->type->itemData(m_ui->type->currentIndex()).toInt()));

    return btSetting.toMap();
}

bool BtWidget::isValid() const
{
    return m_ui->bdaddr->isValid();
}
