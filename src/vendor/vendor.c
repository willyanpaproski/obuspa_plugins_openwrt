/*
 *
 * Copyright (C) 2019-2025, Broadband Forum
 * Copyright (C) 2024-2025, Vantiva Technologies SAS
 * Copyright (C) 2016-2024  CommScope, Inc
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * \file vendor.c
 *
 * Implements the interface to all vendor implemented data model nodes
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "usp_err_codes.h"
#include "vendor_defs.h"
#include "vendor_api.h"
#include "usp_api.h"
#include "plugins/dhcpv4.h"
#include "plugins/ntp.h"
#include "plugins/deviceInfo.h"

/*********************************************************************//**
**
** VENDOR_Init
**
** Initialises this component, and registers all parameters and vendor hooks, which it implements
**
** \param   None
**
** \return  USP_ERR_OK if successful
**
**************************************************************************/
int VENDOR_Init(void)
{
    int ntp_group_id = 1;
    int dhcpv4_group_id = 2;
    int err = USP_ERR_OK;

    //DHCPv4
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Enable", GetPoolEnabled, SetPoolEnabled, NULL, DM_BOOL);
    //err |= USP_REGISTER_Param_Constant("Device.DHCPv4.Server.PoolNumberOfEntries", "1", DM_STRING);
    //err |= USP_REGISTER_Param_Constant("Device.DHCPv4.Server.Pool.{i}.Alias", "cpe-dhcpv4pool", DM_STRING);
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Pool.{i}.DNSServers", GetDnsServers, SetDnsServers, NULL, DM_STRING);
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Pool.{i}.DomainName", GetDomainName, SetDomainName, NULL, DM_STRING);
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Pool.{i}.Enable", GetPoolEnabled, SetPoolEnabled, NULL, DM_BOOL);
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Pool.{i}.IPRouters", GetGateway, SetGateway, NULL, DM_STRING);
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Pool.{i}.LeaseTime", GetLeaseTime, SetLeaseTime, NULL, DM_STRING);
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Pool.{i}.SubnetMask", GetSubnetMask, SetSubnetMask, NULL, DM_STRING);
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Pool.{i}.MaxAddress", GetMaxAddress, SetMaxAddress, NULL, DM_STRING);
    //err |= USP_REGISTER_VendorParam_ReadWrite("Device.DHCPv4.Server.Pool.{i}.MinAddress", GetMinAddress, SetMinAddress, NULL, DM_STRING);
    //err |= USP_REGISTER_VendorParam_ReadOnly("Device.DHCPv4.Server.Pool.{i}.Status", GetDHCPv4Status, DM_STRING);
    
    err |= USP_REGISTER_GroupVendorHooks(dhcpv4_group_id, GetDHCPv4Params, SetDHCPv4Params, NULL, NULL);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Enable", DM_BOOL);
    err |= USP_REGISTER_GroupedVendorParam_ReadOnly(dhcpv4_group_id, "Device.DHCPv4.Server.PoolNumberOfEntries", DM_STRING);

    err |= USP_REGISTER_Object("Device.DHCPv4.Server.Pool.{i}.", ValidateAddPool, NULL, NULL, ValidateRemovePool, NULL, NULL);
    err |= USP_REGISTER_GroupedVendorParam_ReadOnly(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.Alias", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.DNSServers", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.DomainName", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.Enable", DM_BOOL);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.IPRouters", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.LeaseTime", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.SubnetMask", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.MaxAddress", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.MinAddress", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadOnly(dhcpv4_group_id, "Device.DHCPv4.Server.Pool.{i}.Status", DM_STRING);

    //NTP
    err |= USP_REGISTER_GroupVendorHooks(ntp_group_id, GetTimeParams, SetTimeParams, NULL, NULL);
    err |= USP_REGISTER_GroupedVendorParam_ReadOnly(ntp_group_id, "Device.Time.CurrentLocalTime", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(ntp_group_id, "Device.Time.Enable", DM_BOOL);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(ntp_group_id, "Device.Time.LocalTimeZone", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(ntp_group_id, "Device.Time.NTPServer1", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadWrite(ntp_group_id, "Device.Time.NTPServer2", DM_STRING);
    err |= USP_REGISTER_GroupedVendorParam_ReadOnly(ntp_group_id, "Device.Time.Status", DM_STRING);

    //DeviceInfo
    err |= USP_REGISTER_VendorParam_ReadWrite("Device.DeviceInfo.X_IXC_Hostname", GetHostname, SetHostname, NULL, DM_STRING);

    return err;
}


/*********************************************************************//**
**
** VENDOR_Start
**
** Called after data model has been registered and after instance numbers have been read from the USP database
** Typically this function is used to seed the data model with instance numbers or
** initialise internal data structures which require the data model to be running to access parameters
**
** \param   None
**
** \return  USP_ERR_OK if successful
**
**************************************************************************/
int VENDOR_Start(void)
{

    USP_DM_InformInstance("Device.DHCPv4.Server.Pool.1.");

    return USP_ERR_OK;
}

/*********************************************************************//**
**
** VENDOR_Stop
**
** Called when stopping USP agent gracefully, to free up memory and shutdown
** any vendor processes etc
**
** \param   None
**
** \return  USP_ERR_OK if successful
**
**************************************************************************/
int VENDOR_Stop(void)
{

    return USP_ERR_OK;
}

