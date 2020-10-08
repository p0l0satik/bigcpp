/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*************************************************************************
 *
 *  The Contents of this file are made available subject to the terms of
 *  the BSD license.
 *
 *  Copyright 2000, 2010 Oracle and/or its affiliates.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Sun Microsystems, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************/

#include <addon.hxx>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/frame/XFrame.hpp>
#include <com/sun/star/frame/XController.hpp>
#include <com/sun/star/awt/Toolkit.hpp>
#include <com/sun/star/awt/XWindowPeer.hpp>
#include <com/sun/star/awt/WindowAttribute.hpp>
#include <com/sun/star/awt/XMessageBox.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/text/XTextDocument.hpp>
#include <com/sun/star/text/XText.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/text/XTextTable.hpp>
#include <com/sun/star/text/XTextContent.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/table/XTableRows.hpp>
#include <cppuhelper/supportsservice.hxx>
#include <com/sun/star/table/XCellRange.hpp>
#include <com/sun/star/text/XTextTablesSupplier.hpp>

#include <com/sun/star/frame/Desktop.hpp>
#include <rtl/ustring.hxx>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>

using rtl::OUString;
using namespace com::sun::star::uno;
using namespace com::sun::star::frame;
using namespace com::sun::star::awt;
using namespace com::sun::star::lang;
using namespace com::sun::star::text;
using namespace com::sun::star::beans;
using namespace com::sun::star::table;
using namespace com::sun::star::container;
using com::sun::star::beans::PropertyValue;
using com::sun::star::util::URL;

// This is the service name an Add-On has to implement
#define SERVICE_NAME "com.sun.star.frame.ProtocolHandler"


/**
  * Show a message box with the UNO based toolkit
  */
static void ShowMessageBox( const Reference< XToolkit >& rToolkit, const Reference< XFrame >& rFrame, const OUString& aTitle, const OUString& aMsgText )
{
    if ( rFrame.is() && rToolkit.is() )
    {
        // describe window properties.
        WindowDescriptor                aDescriptor;
        aDescriptor.Type              = WindowClass_MODALTOP;
        aDescriptor.WindowServiceName = "infobox";
        aDescriptor.ParentIndex       = -1;
        aDescriptor.Parent            = Reference< XWindowPeer >( rFrame->getContainerWindow(), UNO_QUERY );
        aDescriptor.Bounds            = Rectangle(0,0,300,200);
        aDescriptor.WindowAttributes  = WindowAttribute::BORDER |
WindowAttribute::MOVEABLE |
WindowAttribute::CLOSEABLE;

        Reference< XWindowPeer > xPeer = rToolkit->createWindow( aDescriptor );
        if ( xPeer.is() )
        {
            Reference< XMessageBox > xMsgBox( xPeer, UNO_QUERY );
            if ( xMsgBox.is() )
            {
                xMsgBox->setCaptionText( aTitle );
                xMsgBox->setMessageText( aMsgText );
                xMsgBox->execute();
            }
        }
    }
}

/**
  * Called by the Office framework.
  * One-time initialization. We have to store the context information
  * given, like the frame we are bound to, into our members.
  */
void SAL_CALL Addon::initialize( const Sequence< Any >& aArguments ) throw ( Exception, RuntimeException)
{
    Reference < XFrame > xFrame;
    if ( aArguments.getLength() )
    {
        aArguments[0] >>= xFrame;
        mxFrame = xFrame;
    }

    // Create the toolkit to have access to it later
    mxToolkit = Reference< XToolkit >( Toolkit::create(mxContext), UNO_QUERY_THROW );
}

/**
  * Called by the Office framework.
  * We are ask to query the given URL and return a dispatch object if the URL
  * contains an Add-On command.
  */
Reference< XDispatch > SAL_CALL Addon::queryDispatch( const URL& aURL, const ::rtl::OUString& sTargetFrameName, sal_Int32 nSearchFlags )
                throw( RuntimeException )
{
    Reference < XDispatch > xRet;
    if ( aURL.Protocol.equalsAscii("org.openoffice.Office.addon.example:") )
    {
        if ( aURL.Path.equalsAscii( "Function1" ) )
            xRet = this;
        else if ( aURL.Path.equalsAscii( "Function2" ) )
            xRet = this;
        else if ( aURL.Path.equalsAscii( "Help" ) )
            xRet = this;
    }

    return xRet;
}

/**
  * Called by the Office framework.
  * We are ask to execute the given Add-On command URL.
  */
void SAL_CALL Addon::dispatch( const URL& aURL, const Sequence < PropertyValue >& lArgs ) throw (RuntimeException)
{
    if ( aURL.Protocol.equalsAscii("org.openoffice.Office.addon.example:") )
    {
        if ( aURL.Path.equalsAscii( "Function2" ) )
        {
            std::srand(std::time(nullptr)); 

            //get document
            Reference<XMultiComponentFactory> xServiceManager = mxContext->getServiceManager();
            Reference<XInterface> xDesktop = xServiceManager->createInstanceWithContext(OUString("com.sun.star.frame.Desktop"), mxContext);
            if ( !xServiceManager.is() )
            {
                fprintf(stderr, "no service manager!\n");
                return;
            }

            Reference<XDesktop2> xDesktop2(xDesktop, UNO_QUERY);

            Reference<XComponent> xComponent = xDesktop2->loadComponentFromURL(OUString("private:factory/swriter"), // URL to the ods file
                                                    OUString( "_blank" ), 0,
                                                    Sequence < ::com::sun::star::beans::PropertyValue >());



            Reference < XTextDocument > xTextDocument (xComponent,UNO_QUERY);
            Reference< XText > xText = xTextDocument->getText();
            Reference<XTextRange> xTextRange = xText->getStart();        
            Reference<XMultiServiceFactory> oDocMSF (xTextDocument,UNO_QUERY);

            //create tables
            for(int nTable = 0; nTable < 2 + std::rand()/((RAND_MAX + 1u)/6); nTable++){

                Reference <XTextTable> xTable (oDocMSF->createInstance(
                            OUString::createFromAscii("com.sun.star.text.TextTable")),UNO_QUERY);
                if (!xTable.is()) {
                    printf("Erreur creation XTextTable interface !\n");
                    return;
                }
            
                // Specify that the table
                int cols =  3 + std::rand()/((RAND_MAX + 1u)/3), rows = 3 + std::rand()/((RAND_MAX + 1u)/7);
                xTable->initialize(cols, rows);
                xTextRange = xText->getEnd();
                
                Reference <XTextContent>xTextContent (xTable,UNO_QUERY);
                
                // Insert the table into the document
                xText->insertTextContent(xTextRange, xTextContent,(unsigned char) 0);

                Reference<XCellRange> xRange(xTable, UNO_QUERY); 
                //fill table
                for (int nRow = 0; nRow < cols; ++nRow)
                    for (int nCol = 0; nCol < rows; ++nCol){
                        Reference<XCell> cell = xRange->getCellByPosition( nCol, nRow );
                        Reference<XText> text(cell, UNO_QUERY); 
                        text->setString(OUString::createFromAscii(("row_" + std::to_string(nRow) + " col_"
                                         + std::to_string(nCol)).c_str()));
                    }
            }

        }
        else if ( aURL.Path.equalsAscii( "Function1" ) )
        {
            Reference < XTextDocument > xTextDocument (mxFrame->getController()->getModel(),UNO_QUERY);
            Reference<XTextTablesSupplier> table_supplier(xTextDocument, UNO_QUERY);


            auto all_tables = table_supplier->getTextTables(); 
            auto elements = all_tables->getElementNames();
            for (auto el: elements){

                Reference <XTextTable> xTable(all_tables->getByName(el),UNO_QUERY);
                //calsulate min square matrix
                Reference<XTextTableCursor> pos = xTable->createCursorByCellName("A1");
                int max_col = 0, max_row = 0, squared = 0;
                while(pos->goDown(1, false)) max_row ++;
                while(pos->goRight(1, false)) max_col ++;
                squared = std::min(max_col, max_row);

                Reference<XCellRange> xRange(xTable, UNO_QUERY);

                //transpose 

                for (int nRow = 0; nRow <= squared; ++nRow)
                    for (int nCol = nRow; nCol <= squared; ++nCol){
                        Reference<XCell> one = xRange->getCellByPosition(nCol, nRow);
                        Reference<XCell> two = xRange->getCellByPosition(nRow, nCol);
                        Reference<XText> one_text(one, UNO_QUERY);            
                        Reference<XText> two_text(two, UNO_QUERY);
                        auto buf = one_text->getString();
                        one_text->setString(two_text->getString());
                        two_text->setString(buf);
                    }
            }

        }
        else if ( aURL.Path.equalsAscii( "Help" ) )
        {
            // Show info box
            ShowMessageBox( mxToolkit, mxFrame,
                            OUString( "About SDK Add-On example" ),
                            OUString( "This is the SDK Add-On example" ) );
    }
    }
}

/**
  * Called by the Office framework.
  * We are ask to query the given sequence of URLs and return dispatch objects if the URLs
  * contain Add-On commands.
  */
Sequence < Reference< XDispatch > > SAL_CALL Addon::queryDispatches( const Sequence < DispatchDescriptor >& seqDescripts )
            throw( RuntimeException )
{
    sal_Int32 nCount = seqDescripts.getLength();
    Sequence < Reference < XDispatch > > lDispatcher( nCount );

    for( sal_Int32 i=0; i<nCount; ++i )
        lDispatcher[i] = queryDispatch( seqDescripts[i].FeatureURL, seqDescripts[i].FrameName, seqDescripts[i].SearchFlags );

    return lDispatcher;
}

/**
  * Called by the Office framework.
  * We are ask to query the given sequence of URLs and return dispatch objects if the URLs
  * contain Add-On commands.
  */
void SAL_CALL Addon::addStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL ) throw (RuntimeException)
{
}

/**
  * Called by the Office framework.
  * We are ask to query the given sequence of URLs and return dispatch objects if the URLs
  * contain Add-On commands.
  */
void SAL_CALL Addon::removeStatusListener( const Reference< XStatusListener >& xControl, const URL& aURL ) throw (RuntimeException)
{
}

// Helper functions for the implementation of UNO component interfaces.
OUString Addon_getImplementationName()
throw (RuntimeException)
{
    return OUString ( IMPLEMENTATION_NAME );
}

Sequence< ::rtl::OUString > SAL_CALL Addon_getSupportedServiceNames()
throw (RuntimeException)
{
    Sequence < ::rtl::OUString > aRet(1);
    ::rtl::OUString* pArray = aRet.getArray();
    pArray[0] =  OUString ( SERVICE_NAME );
    return aRet;
}

Reference< XInterface > SAL_CALL Addon_createInstance( const Reference< XComponentContext > & rContext)
    throw( Exception )
{
    return (cppu::OWeakObject*) new Addon( rContext );
}

// Implementation of the recommended/mandatory interfaces of a UNO component.
// XServiceInfo
::rtl::OUString SAL_CALL Addon::getImplementationName(  )
    throw (RuntimeException)
{
    return Addon_getImplementationName();
}

sal_Bool SAL_CALL Addon::supportsService( const ::rtl::OUString& rServiceName )
    throw (RuntimeException)
{
    return cppu::supportsService(this, rServiceName);
}

Sequence< ::rtl::OUString > SAL_CALL Addon::getSupportedServiceNames(  )
    throw (RuntimeException)
{
    return Addon_getSupportedServiceNames();
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
