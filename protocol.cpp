#include "protocol.h"

Protocol::Protocol() {}

/**
 * @brief Loads a byte array from a file.
 * @param fileName File to load from
 * @return Data
 */
QByteArray Protocol::UTF8JsonFromFile(QString fileName) {
    // Temp
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray data = file.readAll();
        return data;
    }
    return {};
}

/**
 * Converts a byte array to a JSON document.
 * If invalid empty/invalid data is received, an empty JSON document is returned.
 * @brief Converts a byte array to a JSON document.
 * @param data Byte array
 * @return JSON document
 */
QJsonDocument Protocol::UTF8ToJsonDocument(const QByteArray &data) {
    if (data.size() == 0)
        return {};
    return QJsonDocument::fromJson(data);
}

/**
 * Converts a JSON document to a byte array.
 * If invalid/empty JSON document is received an empty array is returned.
 * @brief Converts a JSON document to a byte array.
 * @param doc JSON Document to convert
 * @return Byte array
 */
QByteArray Protocol::jsonDocumentToUTF8(const QJsonDocument &doc) {
    if (doc.isNull())
        return {};
    return doc.toJson();
}

/**
 * Converts a JSON document to a system class that contains components.
 * If the JSON document is invalid or contains invalid data, a nullptr is returned!
 * @brief Converts json document to system object.
 * @param doc JSON Document to convert
 * @return System class
 */
System *Protocol::jsonDocumentToSystem(const QJsonDocument &doc) {
    // Parse
    if (doc.isEmpty())
        return nullptr;
    auto system = new System();
    auto object = doc.object();
    for (const auto &componentType: object.keys()) {
        if (componentType == "cpu") {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component: componentTypeValue.keys()) {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto maxFrequency = componentValue.value("maxFrequency");
                auto cores = componentValue.value("cores");
                auto fk_client = componentValue.value("fkClient");
                if (id.isUndefined() || name.isUndefined() || maxFrequency.isUndefined() || cores.isUndefined() ||
                    fk_client.isUndefined()) {
                    delete system;
                    return nullptr;
                }
                if (!maxFrequency.isDouble() || !name.isString()) {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Cpu(maxFrequency.toDouble(),
                                             cores.toInt(),
                                             fk_client.toInt(),
                                             id.toInt(),
                                             name.toString()));
            }
        } else if (componentType == "disk") {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component: componentTypeValue.keys()) {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto capacity = componentValue.value("capacity");
                auto type = componentValue.value("type");
                auto fk_client = componentValue.value("fkClient");
                if (id.isUndefined() || name.isUndefined() || capacity.isUndefined() || type.isUndefined() ||
                    fk_client.isUndefined()) {
                    delete system;
                    return nullptr;
                }
                if (!name.isString() || !capacity.isDouble() || !type.isString()) {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Disk(capacity.isDouble(),
                                              type.toString(),
                                              fk_client.toInt(),
                                              id.toInt(),
                                              name.toString()));
            }
        } else if (componentType == "gpu") {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component: componentTypeValue.keys()) {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto max_frequency = componentValue.value("maxFrequency");
                auto vram = componentValue.value("vRam");
                auto fk_client = componentValue.value("fkClient");
                if (id.isUndefined() || name.isUndefined() || max_frequency.isUndefined() || vram.isUndefined() ||
                    fk_client.isUndefined()) {
                    delete system;
                    return nullptr;
                }
                if (!name.isString() || !max_frequency.isDouble()) {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Gpu(max_frequency.toDouble(),
                                             vram.toDouble(),
                                             fk_client.toInt(),
                                             id.toInt(),
                                             name.toString()));
            }
        } else if (componentType == "network") {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component: componentTypeValue.keys()) {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto type = componentValue.value("type");
                auto fk_client = componentValue.value("fkClient");
                if (id.isUndefined() || name.isUndefined() || type.isUndefined() || fk_client.isUndefined()) {
                    delete system;
                    return nullptr;
                }
                if (!name.isString() || !type.isString()) {
                    delete system;
                    return nullptr;
                }
                system->addComponent(new Network(type.toString(),
                                                 fk_client.toInt(),
                                                 id.toInt(),
                                                 name.toString()));
            }
        } else if (componentType == "ram") {
            auto componentTypeValue = object.value(componentType).toObject();
            for (const auto &component: componentTypeValue.keys()) {
                auto componentValue = componentTypeValue.value(component).toObject();
                auto id = componentValue.value("id");
                auto name = componentValue.value("name");
                auto capacity = componentValue.value("capacity");
                auto type = componentValue.value("type");
                auto frequency = componentValue.value("frequency");
                auto fk_client = componentValue.value("fkClient");
                if (id.isUndefined() || name.isUndefined() || capacity.isUndefined() || type.isUndefined() ||
                    frequency.isUndefined() || fk_client.isUndefined()) {
                    delete system;
                    return nullptr;
                }
                if (!name.isString() || !type.isString() || !frequency.isDouble() || !capacity.isDouble()) {
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

/**
 * Converts a system object to a JSON document. The object should contain at least some components.
 * If the system class is empty, returns an empty document.
 * @brief Converts a system object to a JSON document.
 * @param system System to convert
 * @return JSON document
 */
QJsonDocument Protocol::systemToJsonDocument(const System *system) {
    // Parse
    // For every component in the system, add it to the JSON object
    QJsonObject object;
    for (const auto &component: system->getComponents()) {
        QJsonObject componentObject;
        if (component && System::isCpu(component)) {
            auto cpu = dynamic_cast<const Cpu *>(component);
            componentObject.insert("maxFrequency", cpu->getMaxFrequency());
            componentObject.insert("cores", cpu->getCores());
        } else if (component && System::isDisk(component)) {
            auto disk = dynamic_cast<const Disk *>(component);
            componentObject.insert("capacity", disk->getCapacity());
            componentObject.insert("type", disk->getType());
        } else if (component && System::isGpu(component)) {
            auto gpu = dynamic_cast<const Gpu *>(component);
            componentObject.insert("maxFrequency", gpu->getMaxFrequency());
            componentObject.insert("vRam", gpu->getVRam());
        } else if (component && System::isNetwork(component)) {
            auto network = dynamic_cast<const Network *>(component);
            componentObject.insert("type", network->getType());
        } else if (component && System::isRam(component)) {
            auto ram = dynamic_cast<const Ram *>(component);
            componentObject.insert("capacity", ram->getCapacity());
            componentObject.insert("type", ram->getType());
            componentObject.insert("frequency", ram->getFrequency());
        } else {
            continue;
        }
    }
    return {};
}
