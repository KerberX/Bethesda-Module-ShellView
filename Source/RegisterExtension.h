#pragma once
#include "BethesdaModule.hpp"
#include <Kx/General/String.h>
#include <Kx/General/NativeUUID.h>
#include <Kx/General/UniversallyUniqueID.h>
#include <Kx/FileSystem/FSPath.h>
#include <Kx/System/ErrorCodeValue.h>
#include <Kx/System/Registry.h>

namespace BethesdaModule::ShellView
{
	// production code should use an installer technology like MSI to register its handlers rather than this class.
	// This class is used for demonstration purposes, it encapsulate the different types of handler registrations,
	// schematics those by proving methods that have parameters that map to the supported extension schema and makes
	// it easy to create self registering .exe and .dlls.

	class RegisterExtension final
	{
		private:
			FSPath m_ModuleName;
			UniversallyUniqueID m_ClassGUID;
			RegistryBaseKey m_RegistryBaseKey;
			mutable bool m_AssociationsChanged = false;

		private:
			HResult _EnsureModule() const
			{
				return !m_ModuleName.IsEmpty() ? S_OK : E_FAIL;
			}
			bool _IsBaseClassProgID(const String& progID)  const;
			HResult _EnsureBaseProgIDVerbIsNone(const String& progID) const;
			void _UpdateAssocChanged(HResult hr, const String& keyFormatString) const;

		public:
			RegisterExtension(const UniversallyUniqueID& clsid = {}, RegistryBaseKey hkeyRoot = RegistryBaseKey::CurrentUser);
			~RegisterExtension();

		public:
			bool HasClassID() const
			{
				return !m_ClassGUID.IsNull();
			};
			void SetHandlerCLSID(const UniversallyUniqueID& clsid)
			{
				m_ClassGUID = clsid;
			}
			void SetInstallScope(RegistryBaseKey hkeyRoot)
			{
				// Must be 'RegistryBaseKey::CurrentUser' or 'RegistryBaseKey::LocalMachine'.
				m_RegistryBaseKey = hkeyRoot;
			}

			HResult SetModule(String moduleName)
			{
				m_ModuleName = std::move(moduleName);
			}
			HResult SetModule(void* handle);

			HResult RegisterInProcServer(const String& friendlyName, const String& threadingModel) const;
			HResult RegisterInProcServerAttribute(const String& attribute, uint32_t value) const;

			// Register the COM local server for the current running module this is for self registering applications
			HResult RegisterAppAsLocalServer(const String& friendlyName, const String& cmdLine = {}) const;
			HResult RegisterElevatableLocalServer(const String& friendlyName, uint32_t idLocalizeString, uint32_t idIconRef) const;
			HResult RegisterElevatableInProcServer(const String& friendlyName, uint32_t idLocalizeString, uint32_t idIconRef) const;

			// Remove a COM object registration
			HResult UnRegisterObject() const;

			// This enables drag drop directly onto the .exe, useful if you have a shortcut to the .exe somewhere (or the .exe is accessible via the send to menu)
			HResult RegisterAppDropTarget() const;

			// Create registry entries for drop target based static verb. The specified class ID will be (will be what?)
			HResult RegisterCreateProcessVerb(const String& progID, const String& verb, const String& cmdLine, const String& verbDisplayName) const;
			HResult RegisterDropTargetVerb(const String& progID, const String& verb, const String& verbDisplayName) const;
			HResult RegisterExecuteCommandVerb(const String& progID, const String& verb, const String& verbDisplayName) const;
			HResult RegisterExplorerCommandVerb(const String& progID, const String& verb, const String& verbDisplayName) const;
			HResult RegisterExplorerCommandStateHandler(const String& progID, const String& verb) const;
			HResult RegisterVerbAttribute(const String& progID, const String& verb, const String& valueName) const;
			HResult RegisterVerbAttribute(const String& progID, const String& verb, const String& valueName, const String& value) const;
			HResult RegisterVerbAttribute(const String& progID, const String& verb, const String& valueName, uint32_t value) const;
			HResult RegisterVerbDefaultAndOrder(const String& progID, const String& verbOrderFirstIsDefault) const;
			HResult RegisterPlayerVerbs(const std::vector<String> associations, const String& verb, const String& title) const;

			HResult UnRegisterVerb(const String& progID, const String& verb) const;
			HResult UnRegisterVerbs(const std::vector<String> associations, const String& verb) const;

			HResult RegisterContextMenuHandler(const String& progID, const String& description) const;
			HResult RegisterRightDragContextMenuHandler(const String& progID, const String& description) const;

			HResult RegisterAppShortcutInSendTo() const;
			HResult RegisterThumbnailHandler(const String& extension) const;
			HResult RegisterPropertyHandler(const String& extension) const;
			HResult RegisterLinkHandler(const String& progID) const;
			HResult UnRegisterPropertyHandler(const String& extension) const;

			HResult RegisterProgID(const String& progID, const String& typeName, uint32_t idIcon) const;
			HResult UnRegisterProgID(const String& progID, const String& fileExtension) const;
			HResult RegisterExtensionWithProgID(const String& fileExtension, const String& progID) const;
			HResult RegisterOpenWith(const String& fileExtension, const String& progID) const;
			HResult RegisterNewMenuNullFile(const String& fileExtension, const String& progID) const;
			HResult RegisterNewMenuData(const String& fileExtension, const String& progID, PCSTR pszBase64) const;
			HResult RegisterKind(const String& fileExtension, PCWSTR pszKindValue) const;
			HResult UnRegisterKind(const String& fileExtension) const;

			HResult RegisterPropertyHandlerOverride(PCWSTR pszProperty) const;
			HResult RegisterHandlerSupportedProtocols(const String& protocol) const;

			HResult RegisterProgIDValue(const String& progID, const String& valueName) const;
			HResult RegisterProgIDValue(const String& progID, const String& valueName, const String& value) const;
			HResult RegisterProgIDValue(const String& progID, const String& valueName, uint32_t dwValue) const;
	};
}