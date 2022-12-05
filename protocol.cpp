#include "protocol.h"

Protocol::Protocol() = default;

/**
 * Loads a byte array from a file.
 * @param fileName File to load from
 * @return Data
 */
QByteArray Protocol::UTF8JsonFromFile(const QString &fileName) {
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
 * @param doc JSON Document to convert
 * @return System class
 */
System *Protocol::jsonDocumentToSystem(const QJsonDocument &doc) {
    // Parse
    if (doc.isNull())
        return nullptr;
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
                auto maxFrequency = componentValue.value("max_frequency");
                auto cores = componentValue.value("cores");
                auto fk_client = componentValue.value("fk_client");
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
                auto fk_client = componentValue.value("fk_client");
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
                auto max_frequency = componentValue.value("max_frequency");
                auto vram = componentValue.value("vram");
                auto fk_client = componentValue.value("fk_client");
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
                auto fk_client = componentValue.value("fk_client");
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
                auto fk_client = componentValue.value("fk_client");
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
 * @param system System to convert
 * @return JSON document
 */
QJsonDocument Protocol::systemToJsonDocument(System *system) {
    // For every component in the system, add it to the JSON object
    QJsonObject object;
    for (const auto &component: system->getComponents()) {
        if (component && System::isCpu(component)) {
            auto cpu = dynamic_cast<const Cpu *>(component);
            QJsonObject cpuObject;
            cpuObject.insert("id", cpu->getId());
            cpuObject.insert("name", cpu->getName());
            cpuObject.insert("max_frequency", cpu->getMaxFrequency());
            cpuObject.insert("cores", cpu->getCores());
            cpuObject.insert("fk_client", cpu->getFkClient());
            if (object.contains("cpu")) {
                auto temp = object["cpu"].toObject();
                temp.insert("cpu_" + QString::number(temp.size()), cpuObject);
                object.remove("cpu");
                object.insert("cpu", temp);
            } else {
                QJsonObject comp_type;
                comp_type.insert("cpu_0", cpuObject);
                object.insert("cpu", comp_type);
            }
        } else if (component && System::isDisk(component)) {
            auto disk = dynamic_cast<const Disk *>(component);
            QJsonObject diskObject;
            diskObject.insert("id", disk->getId());
            diskObject.insert("name", disk->getName());
            diskObject.insert("capacity", disk->getCapacity());
            diskObject.insert("type", disk->getType());
            diskObject.insert("fk_client", disk->getFkClient());
            if (object.contains("disk")) {
                auto temp = object["disk"].toObject();
                temp.insert("disk_" + QString::number(temp.size()), diskObject);
                object.remove("disk");
                object.insert("disk", temp);
            } else {
                QJsonObject comp_type;
                comp_type.insert("disk_0", diskObject);
                object.insert("disk", comp_type);
            }
        } else if (component && System::isGpu(component)) {
            auto gpu = dynamic_cast<const Gpu *>(component);
            QJsonObject gpuObject;
            gpuObject.insert("id", gpu->getId());
            gpuObject.insert("name", gpu->getName());
            gpuObject.insert("max_frequency", gpu->getMaxFrequency());
            gpuObject.insert("vram", gpu->getVRam());
            gpuObject.insert("fk_client", gpu->getFkClient());
            if (object.contains("gpu")) {
                auto temp = object["gpu"].toObject();
                temp.insert("gpu_" + QString::number(temp.size()), gpuObject);
                object.remove("gpu");
                object.insert("gpu", temp);
            } else {
                QJsonObject comp_type;
                comp_type.insert("gpu_0", gpuObject);
                object.insert("gpu", comp_type);
            }
        } else if (component && System::isNetwork(component)) {
            auto network = dynamic_cast<const Network *>(component);
            QJsonObject networkObject;
            networkObject.insert("id", network->getId());
            networkObject.insert("name", network->getName());
            networkObject.insert("type", network->getType());
            networkObject.insert("fk_client", network->getFkClient());
            if (object.contains("network")) {
                auto temp = object["network"].toObject();
                temp.insert("network_" + QString::number(temp.size()), networkObject);
                object.remove("network");
                object.insert("network", temp);
            } else {
                QJsonObject comp_type;
                comp_type.insert("network_0", networkObject);
                object.insert("network", comp_type);
            }
        } else if (component && System::isRam(component)) {
            auto ram = dynamic_cast<const Ram *>(component);
            QJsonObject ramObject;
            ramObject.insert("id", ram->getId());
            ramObject.insert("name", ram->getName());
            ramObject.insert("capacity", ram->getCapacity());
            ramObject.insert("type", ram->getType());
            ramObject.insert("frequency", ram->getFrequency());
            ramObject.insert("fk_client", ram->getFkClient());
            if (object.contains("ram")) {
                auto temp = object["ram"].toObject();
                temp.insert("ram_" + QString::number(temp.size()), ramObject);
                object.remove("ram");
                object.insert("ram", temp);
            } else {
                QJsonObject comp_type;
                comp_type.insert("ram_0", ramObject);
                object.insert("ram", comp_type);
            }
        } else {
            continue;
        }
    }
    return QJsonDocument(object);
}

/**
 * Saves JSON data to the specified file.
 * @param json JSON data
 * @param fileName File name to save to
 */
void Protocol::UTF8JsonToFile(QByteArray &json, const QString &fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(json);
        file.close();
    }
}

/**
 * Reads a username from a specified JSON file.
 * If a username is not found / JSON is invalid, returns an empty string.
 * @param fileName File name to read from
 * @return Username
 */
QByteArray Protocol::getUsernameFromJson(QByteArray &json) {
    QJsonDocument document = QJsonDocument::fromJson(json);
    if (document.isObject()) {
        QJsonObject object = document.object();
        if (object.contains("username")) {
            return object["username"].toString().toUtf8();
        }
    }
    return {};
}

/**
 * Reads a password from a specified JSON file.
 * If a password is not found / JSON is invalid, returns an empty string.
 * @param fileName File name to read from
 * @return Password
 */
QByteArray Protocol::getPasswordFromJson(QByteArray &json) {
    QJsonDocument document = QJsonDocument::fromJson(json);
    if (document.isObject()) {
        QJsonObject object = document.object();
        if (object.contains("password")) {
            return object["password"].toString().toUtf8();
        }
    }
    return {};
}

/**
 * Creates an authentication JSON from the specified username and password.
 * @param username Username
 * @param password Password
 * @return JSON file
 */
QByteArray Protocol::getAuthenticationJson(QByteArray &username, QByteArray &password) {
    QJsonObject object;
    object.insert("username", QString::fromUtf8(username));
    object.insert("password", QString::fromUtf8(password));
    return QJsonDocument(object).toJson();
}

/**
 * Reads a client ID from a specified JSON file.
 * If a client ID is not found, an empty string is returned.
 * @param json JSON file to read from
 * @return Client ID
 */
int Protocol::getClientIDFromJson(QByteArray &json) {
    QJsonDocument document = QJsonDocument::fromJson(json);
    if (document.isObject()) {
        QJsonObject object = document.object();
        if (object.contains("id")) {
            return object["id"].toString().toInt();
        }
    }
    return {};
}

/**
 * Creates an identification JSON from the given client ID.
 * @param clientID Client ID
 * @return Client Identification JSON
 */
QByteArray Protocol::getClientIdentificationJson(int &clientID) {
    QJsonObject object;
    object.insert("id", clientID);
    return QJsonDocument(object).toJson();
}

/**
 * Creates confirmation JSON from the given confirmation (true/false).
 * @param confirmation Confirmation
 * @return Confirmation JSON
 */
QByteArray Protocol::getConfirmationJson(bool &confirmation) {
    QJsonObject object;
    object.insert("confirmation", confirmation);
    return QJsonDocument(object).toJson();
}

/**
 * Return an error message.
 * @param error
 * @return
 */
QByteArray Protocol::getErrorJson(QString &error) {
    return error.toUtf8();
}
