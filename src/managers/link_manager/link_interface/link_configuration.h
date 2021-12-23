#pragma once

#include <memory>

#include <QSettings>


namespace communication
{
    class Link;
    class LinkConfiguration : public QObject
    {
        Q_OBJECT

        enum class LinkType : uint8_t
        {
            TypeSerial,
            TypeCan,
            TypeUdp,
            TypeTcp,
            TypeLast
        };

        Q_SIGNALS:
            void nameChanged(const QString& name);
            void dynamicChanged();
            void autoConnectChanged();
            void highLatencyChanged();
            void linkChanged();
        
        public:
            Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
            Q_PROPERTY(Link* link READ getLink NOTIFY linkChanged)
            Q_PROPERTY(LinkType linkType READ getType CONSTANT)
            Q_PROPERTY(bool dynamic READ getDynamic WRITE setDynamic NOTIFY dynamicChanged)
            Q_PROPERTY(bool autoConnect READ getAutoConnect  WRITE setAutoConnect NOTIFY autoConnectChanged)
            Q_PROPERTY(QString settingsURL READ getSettingsURL CONSTANT)
            Q_PROPERTY(QString settingsTitle READ getSettingsTitle CONSTANT)
            Q_PROPERTY(bool highLatency READ getHighLatency  WRITE setHighLatency NOTIFY highLatencyChanged)

            Q_ENUM(LinkType)

        public:
            LinkConfiguration(const QString& name);
            LinkConfiguration(LinkConfiguration &obj);
            virtual LinkConfiguration& operator=(const LinkConfiguration& src);
            virtual ~LinkConfiguration() {}

            virtual void loadSettings(QSettings& settings, const QString& root) = 0;
            virtual void saveSettings(QSettings& settings, const QString& root) = 0;

            virtual LinkType getType() = 0;
            virtual QString getSettingsURL() = 0;
            virtual QString getSettingsTitle() = 0;

            void setLink(SharedLinkPtr link) { m_p_link = link; Q_EMIT linkChanged(); }
            void setName(const QString &name) { m_name = name; Q_EMIT nameChanged(); }
            void setDynamic(bool dynamic = true) { m_dynamic = dynamic; Q_EMIT dynamicChanged(); }
            void setAutoConnect(bool autoc = true) { m_auto_connect = autoc; Q_EMIT autoConnectChanged(); }
            void setHighLatency(bool hl = false) { m_high_latency = hl; Q_EMIT highLatencyChanged(); }

            Link* getLink(void)  { return m_p_link.lock().get(); }
            QString getName(void) const { return m_name; }
            bool getDynamic() const{ return m_dynamic; }
            bool getAutoConnect() const{ return m_auto_connect; }
            bool getHighLatency() const{ return m_high_latency; }

            static const QString getSettingsRoot();
            static LinkConfiguration* createSettings(int type, const QString& name);
            static LinkConfiguration* duplicateSettings(LinkConfiguration &src);

        protected:
            WeakLinkPtr m_p_link;

        private:
            QString m_name;
            bool m_dynamic;
            bool m_auto_connect;
            bool m_high_latency;
    };
    using SharedLinkConfigurationPtr = std::shared_ptr<LinkConfiguration>;
    using WeakLinkConfigurationPtr = std::weak_ptr<LinkConfiguration>;
}