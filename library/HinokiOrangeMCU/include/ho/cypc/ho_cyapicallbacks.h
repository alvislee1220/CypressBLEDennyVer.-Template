/* SPDX-License-Identifier: MIT */
#ifndef ___HO_CYAPICALLBACKS_H___
#define ___HO_CYAPICALLBACKS_H___

/* HARD FAULT SUPPORT ------------------------------------------------------- */
#define CY_BOOT_INT_DEFAULT_HANDLER_EXCEPTION_ENTRY_CALLBACK
extern void CyBoot_IntDefaultHandler_Exception_EntryCallback(void);

#define CY_BOOT_INT_DEFAULT_HANDLER_ENOMEM_EXCEPTION_CALLBACK
extern void CyBoot_IntDefaultHandler_Enomem_Exception_Callback(void);

#endif /* ___HO_CYAPICALLBACKS_H___ */
