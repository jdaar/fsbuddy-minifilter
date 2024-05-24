#include <fltKernel.h>

#define FSB_FILTER_NAME L"FSBuddyFilter"

typedef struct _FILTER_DATA {
	PFLT_FILTER FilterHandle;
} FILTER_DATA, *PFILTER_DATA;

DRIVER_INITIALIZE DriverEntry;

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
);

NTSTATUS DriverUnload(
	_In_ FLT_FILTER_UNLOAD_FLAGS Flags
);

NTSTATUS DriverQueryTeardown(
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS FSB_PreIRPRead(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);

FLT_POSTOP_CALLBACK_STATUS FSB_PostIRPRead(
	_Inout_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS FltObjects,
	_In_opt_ PVOID CompletionContext,
	_In_ FLT_POST_OPERATION_FLAGS Flags
);

FILTER_DATA FilterData;

CONST FLT_REGISTRATION FilterRegistration = {
    sizeof(FLT_REGISTRATION),
    FLT_REGISTRATION_VERSION,           //  Version
    0,                                  //  Flags
    NULL,                               //  Context
    NULL,                               //  Operation callbacks
    DriverUnload,                       //  FilterUnload
    NULL,                               //  InstanceSetup
    DriverQueryTeardown,                //  InstanceQueryTeardown
    NULL,                               //  InstanceTeardownStart
    NULL,                               //  InstanceTeardownComplete
    NULL,                               //  GenerateFileName
    NULL,                               //  GenerateDestinationFileName
    NULL                                //  NormalizeNameComponent
};

CONST FLT_OPERATION_REGISTRATION FilterCallbacks[] = {
    {
        IRP_MJ_READ,       // IRP message
        0,                 // IRP context
        FSB_PreIRPRead,    // Pre-callback
        FSB_PostIRPRead    // Post-callback
    }
};

#ifndef PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, FSB_Unload)
#pragma alloc_text(PAGE, FSB_QueryTeardown)
#endif

NTSTATUS DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
) {
    NTSTATUS status = STATUS_SUCCESS;

	status = FltRegisterFilter(
		DriverObject,
		&FilterRegistration,
		&FilterData.FilterHandle
	);
	
	if (!NT_SUCCESS(status)) {
        FltUnregisterFilter(FilterData.FilterHandle);
        return status;
    }

    status = FltStartFiltering(FilterData.FilterHandle);

	if (!NT_SUCCESS(status)) {
		FltUnregisterFilter(FilterData.FilterHandle);
	}

	return status;
}

NTSTATUS DriverUnload(
    _In_ FLT_FILTER_UNLOAD_FLAGS Flags
) {
    UNREFERENCED_PARAMETER(Flags);

    PAGED_CODE();

    FltUnregisterFilter(FilterData.FilterHandle);

    return STATUS_SUCCESS;
}

NTSTATUS DriverQueryTeardown(
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
) {
    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(FltObjects);

    PAGED_CODE();

    return STATUS_SUCCESS;
}

FLT_PREOP_CALLBACK_STATUS FSB_PreIRPRead(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID* CompletionContext
) {
	DbgPrint("FSBuddy: %wZ\n", &Data->Iopb->TargetFileObject->FileName);
  
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS FSB_PostIRPRead(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID CompletionContext,
    _In_ FLT_POST_OPERATION_FLAGS Flags
) {
	DbgPrint("FSBuddy: %wZ\n", &Data->Iopb->TargetFileObject->FileName);
  
    return FLT_POSTOP_FINISHED_PROCESSING;
}
