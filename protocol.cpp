#include "protocol.h"

Protocol::Protocol()
{}

QByteArray Protocol::UTF8JsonFromFile(QString fileName)
{
    // Temp
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = file.readAll();
        return data;
    }
    return {};
}

QJsonDocument Protocol::UTF8ToJsonDocument(const QByteArray &data)
{
    return QJsonDocument::fromJson(data);
}

QByteArray Protocol::jsonDocumentToUTF8(const QJsonDocument &doc)
{
    return doc.toJson();
}

// Returns nullptr if the document is invalid
// This function will only work with system, upon checking for errors, will return nullptr
System *Protocol::jsonDocumentToSystem(const QJsonDocument &doc)
{
    // Parse
    if (doc.isEmpty())
        return nullptr;
    auto system = new System();
    auto object = doc.object();
    for (const auto &componentType : object.keys())
    {
        if (componentType == "cpu")
        {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component : componentTypeValue.keys())
            {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto maxFrequency = componentValue.value("max_frequency");
                auto cores = componentValue.value("cores");
                auto fk_client = componentValue.value("fk_client");
                if (id.isUndefined() || name.isUndefined() || maxFrequency.isUndefined() || cores.isUndefined() || fk_client.isUndefined())
                {
                    delete system;
                    return nullptr;
                }
                if (!maxFrequency.isDouble() || !name.isString())
                {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Cpu(maxFrequency.toDouble(),
                                             cores.toInt(),
                                             fk_client.toInt(),
                                             id.toInt(),
                                             name.toString()));
            }
        }
        else if (componentType == "disk")
        {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component : componentTypeValue.keys())
            {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto capacity = componentValue.value("capacity");
                auto type = componentValue.value("type");
                auto fk_client = componentValue.value("fk_client");
                if (id.isUndefined() || name.isUndefined() || capacity.isUndefined() || type.isUndefined() || fk_client.isUndefined())
                {
                    delete system;
                    return nullptr;
                }
                if (!name.isString() || !capacity.isDouble() || !type.isString())
                {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Disk(capacity.isDouble(),
                                              type.toString(),
                                              fk_client.toInt(),
                                              id.toInt(),
                                              name.toString()));
            }
        }
        else if (componentType == "gpu")
        {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component : componentTypeValue.keys())
            {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto max_frequency = componentValue.value("max_frequency");
                auto vram = componentValue.value("vram");
                auto fk_client = componentValue.value("fk_client");
                if (id.isUndefined() || name.isUndefined() || max_frequency.isUndefined() || vram.isUndefined() || fk_client.isUndefined())
                {
                    delete system;
                    return nullptr;
                }
                if (!name.isString() || !max_frequency.isDouble())
                {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Gpu(max_frequency.toDouble(),
                                             vram.toDouble(),
                                             fk_client.toInt(),
                                             id.toInt(),
                                             name.toString()));
            }
        }
        else if (componentType == "network")
        {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component : componentTypeValue.keys())
            {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto type = componentValue.value("type");
                auto fk_client = componentValue.value("fk_client");
                if (id.isUndefined() || name.isUndefined() || type.isUndefined() || fk_client.isUndefined())
                {
                    delete system;
                    return nullptr;
                }
                if (!name.isString() || !type.isString())
                {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Network(type.toString(),
                                                 fk_client.toInt(),
                                                 id.toInt(),
                                                 name.toString()));
            }
        }
        else if (componentType == "ram")
        {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component : componentTypeValue.keys())
            {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto capacity = componentValue.value("capacity");
                auto type = componentValue.value("type");
                auto frequency = componentValue.value("frequency");
                auto fk_client = componentValue.value("fk_client");
                if (id.isUndefined() || name.isUndefined() || capacity.isUndefined() || type.isUndefined() || frequency.isUndefined() || fk_client.isUndefined())
                {
                    delete system;
                    return nullptr;
                }
                if (!name.isString() || !type.isString() || !frequency.isDouble() || !capacity.isDouble())
                {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Ram(capacity.toDouble(),
                                             type.toString(),
                                             frequency.toDouble(),
                                             fk_client.toInt(),
                                             id.toInt(),
                                             name.toString()));
            }
        }
    }
    return system;
}

QJsonDocument Protocol::systemToJsonDocument(const System *system)
{
    // Parse

    return {};
}
