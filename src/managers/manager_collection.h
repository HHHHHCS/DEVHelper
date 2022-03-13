#pragma once

#include <memory>

#include <QObject>


namespace managers
{
    class LinkManager;
    class MainManager;
    class ParametersManager;
    class UpgradeManager;
    class ManagerCollection final : public QObject
    {
        Q_OBJECT

        friend class DevHelperApplication;

        public:
            explicit ManagerCollection(QApplication *app);
            ManagerCollection(const ManagerCollection&) = delete;
            ManagerCollection(const ManagerCollection&&) = delete;
            ManagerCollection& operator=(const ManagerCollection&) = delete;
            ManagerCollection& operator=(const ManagerCollection&&) = delete;
            ~ManagerCollection() final = default;

            std::shared_ptr<LinkManager> link_man() const { return m_p_link_man; }
            std::shared_ptr<MainManager> main_man() const { return m_p_main_man; }
            std::shared_ptr<ParametersManager> param_man() const { return m_p_param_man; }
            std::shared_ptr<UpgradeManager> upgrade_man() const { return m_p_upgrade_man; }

        private:
            std::shared_ptr<LinkManager> m_p_link_man;
            std::shared_ptr<MainManager> m_p_main_man;
            std::shared_ptr<ParametersManager> m_p_param_man;
            std::shared_ptr<UpgradeManager> m_p_upgrade_man;
    };

    class Manager : public QObject
    {
        Q_OBJECT

        public:
            explicit Manager(QApplication *app, ManagerCollection *man_collect);
            Manager(const Manager&) = delete;
            Manager(const Manager&&) = delete;
            Manager& operator=(const Manager&) = delete;
            Manager& operator=(const Manager&&) = delete;
            virtual ~Manager() override = default;

        protected:
            QApplication const* app() const { return m_app; }
            ManagerCollection const* man_collect() const { return m_man_collect; }

        private:
            const QApplication const* m_app;
            const ManagerCollection const* m_man_collect;
    };
}   // namespace managers