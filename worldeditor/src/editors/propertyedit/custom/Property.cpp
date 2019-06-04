#include <QMetaProperty>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPainter>

#include "Property.h"

#include "../editors/Actions.h"

#include "../nextobject.h"

#include <limits.h>
#include <float.h>

Property::Property(const QString &name, QObject *propertyObject, QObject *parent, bool root) :
        QObject(parent),
        m_propertyObject(propertyObject),
        m_Editor(nullptr),
        m_Root(root) {

    QStringList list    = name.split('/');

    m_name  = list.back();
    setObjectName(name);
}

QVariant Property::value(int role) const {
    if(m_propertyObject && role != Qt::DecorationRole) {
        return m_propertyObject->property(qPrintable(objectName()));
    }
    return QVariant();
}

void Property::setValue(const QVariant &value) {
    if(m_propertyObject) {
        m_propertyObject->setProperty(qPrintable(objectName()), value);
    }
}

QString Property::name() const {
    if(m_name.length() != 0)
        return m_name;

    return objectName();
}

bool Property::isReadOnly() const {
    if(m_propertyObject && m_propertyObject->dynamicPropertyNames().contains( objectName().toLocal8Bit() )) {
        return false;
    } else if(m_propertyObject && m_propertyObject->metaObject()->property(m_propertyObject->metaObject()->indexOfProperty(qPrintable(objectName()))).isWritable()) {
        return false;
    } else {
        return true;
    }
}

QWidget *Property::createEditor(QWidget *parent, const QStyleOptionViewItem &) {
    NextObject *next    = dynamic_cast<NextObject *>(m_propertyObject);
    if(m_Root && next) {
        Actions *act    = new Actions(parent);
        act->setMenu(next->menu(m_name));
        m_Editor        = act;
    }
    return m_Editor;
}

QSize Property::sizeHint(const QSize &size) const {
    return size;
}

bool Property::setEditorData(QWidget *, const QVariant &) {
    return false;
}

QVariant Property::editorData(QWidget *) {
    return QVariant();
}

Property *Property::findPropertyObject(QObject *propertyObject) {
    if (m_propertyObject == propertyObject)
        return this;
    for (int i = 0; i < children().size(); ++i) {
        Property *child  = static_cast<Property *>(children()[i])->findPropertyObject(propertyObject);
        if (child)
            return child;
    }
    return nullptr;
}
