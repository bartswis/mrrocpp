///////////////////////////////////////////////////////////
//  ecp_vis_pb_eih_irp6ot.cpp
//  Implementation of the Class ecp_vis_pb_eih_irp6ot
//  Created on:      04-sie-2008 14:26:04
//  Original author: tkornuta
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "common/typedefs.h"
#include "common/impconst.h"
#include "common/com_buf.h"

#include "lib/srlib.h"

#include "ecp/irp6_on_track/ecp_local.h"
#include "ecp/irp6_on_track/ecp_vis_pb_eih_irp6ot.h"


ecp_vis_pb_eih_irp6ot::ecp_vis_pb_eih_irp6ot(ecp_task& _ecp_task, int step) : ecp_visual_servo(_ecp_task){

	measure_border_u[0]=ecp_t.config.return_double_value("measure_border_u0");
	measure_border_u[1]=ecp_t.config.return_double_value("measure_border_u1");
	measure_border_u[2]=ecp_t.config.return_double_value("measure_border_u2");
	measure_border_u[3]=ecp_t.config.return_double_value("measure_border_u3");
	measure_border_u[4]=ecp_t.config.return_double_value("measure_border_u4");
	measure_border_u[5]=ecp_t.config.return_double_value("measure_border_u5");

	measure_border_d[0]=ecp_t.config.return_double_value("measure_border_d0");
	measure_border_d[1]=ecp_t.config.return_double_value("measure_border_d1");
	measure_border_d[2]=ecp_t.config.return_double_value("measure_border_d2");
	measure_border_d[3]=ecp_t.config.return_double_value("measure_border_d3");
	measure_border_d[4]=ecp_t.config.return_double_value("measure_border_d4");
	measure_border_d[5]=ecp_t.config.return_double_value("measure_border_d5");

	d_u_max[0]=ecp_t.config.return_double_value("d_u_max0");
	d_u_max[1]=ecp_t.config.return_double_value("d_u_max1");
	d_u_max[2]=ecp_t.config.return_double_value("d_u_max2");
	d_u_max[3]=ecp_t.config.return_double_value("d_u_max3");
	d_u_max[4]=ecp_t.config.return_double_value("d_u_max4");
	d_u_max[5]=ecp_t.config.return_double_value("d_u_max5");

	d2_u_max[0]=ecp_t.config.return_double_value("d2_u_max0");
	d2_u_max[1]=ecp_t.config.return_double_value("d2_u_max1");
	d2_u_max[2]=ecp_t.config.return_double_value("d2_u_max2");
	d2_u_max[3]=ecp_t.config.return_double_value("d2_u_max3");
	d2_u_max[4]=ecp_t.config.return_double_value("d2_u_max4");
	d2_u_max[5]=ecp_t.config.return_double_value("d2_u_max5");

	gain[0]=ecp_t.config.return_double_value("gain0");
	gain[1]=ecp_t.config.return_double_value("gain1");
	gain[2]=ecp_t.config.return_double_value("gain2");
	gain[3]=ecp_t.config.return_double_value("gain3");
	gain[4]=ecp_t.config.return_double_value("gain4");
	gain[5]=ecp_t.config.return_double_value("gain5");
	
	x2g=ecp_t.config.return_double_value("x2g");

}



ecp_vis_pb_eih_irp6ot::~ecp_vis_pb_eih_irp6ot(){

}





void ecp_vis_pb_eih_irp6ot::next_step_without_constraints(){

	the_robot->EDP_data.set_type = ARM_DV;
	the_robot->EDP_data.instruction_type = SET_GET;

	//G_Tx_G2.set_xyz_euler_zyz( 0,0,0, 0.002, 1.481+0.03, 2.341);	//jesli chwytamy po przekatnej
	G_Tx_G2.set_xyz_euler_zyz( 0,0,0, 0.00, 1.57, 3.141); // jesli chwytak na plasko
	


	if (node_counter==1)
	{
		vsp_vis_sac->base_period=1;
		vsp_vis_sac->current_period=1; 
	}

	if (node_counter==1)
	{
		std::cout << std::endl << std::endl << std::endl << std::endl
				<< std::endl;

		O_Tx_E.set_frame_tab(the_robot->EDP_data.current_arm_frame); // zarem
		std::cout << "YYY " << O_Tx_E << std::endl;

		O_Tx_E.get_xyz_angle_axis(O_r_E[0]);
		O_Tx_E=O_Tx_E*!G_Tx_G2;

		std::cout << "MMM " << node_counter << std::endl;
		std::cout << "YYY " << O_Tx_E << std::endl;

		for (int i=0; i<6; i++)
		{

			O_r_Ep[0][i]=O_r_E[0][i];
			O_r_Ep[1][i]=O_r_E[0][i];
			O_r_Ep[2][i]=O_r_E[0][i];

			O_r_Ep_d[i][1]=0;
			O_r_Ep_d[i][2]=0;
		}

	
		std::cout << node_counter
				<<"------------------------------------------------------------------"
				<< std::endl;
	}
	
	//EIH
	C_Tx_G.set_xyz_rpy(vsp_vis_sac->image.vis_sac.frame_E_r_G__f[0],
			vsp_vis_sac->image.vis_sac.frame_E_r_G__f[1],
			-vsp_vis_sac->image.vis_sac.frame_E_r_G__f[2], // kalib Y w O
			-vsp_vis_sac->image.vis_sac.frame_E_r_G__f[3], 0, 0); 	
	
	G_Tx_S.set_xyz_rpy(x2g, 0, 0, 0, 0, 0);

	O_Tx_E.set_frame_tab(the_robot->EDP_data.current_arm_frame);
	O_Tx_E=O_Tx_E*!G_Tx_G2;
	O_Tx_E.get_xyz_angle_axis(O_r_E[0]);
	
	//jesli nie widzi kostki bo jest za blisko zostaw stare namiary - ?????
	C_Tx_G.get_xyz_angle_axis(C_r_G[0]);
	
	C_Tx_G=C_Tx_G*G_Tx_S;	//CZY NA PEWNO - ??? - czy nie przesunie w X wzlgedem CEIH zamiast oddalic sie od kostki
	O_Tx_G=O_Tx_E*C_Tx_G; //rota O_Tx_E 0,0,0 //E_Tx_CEIH=1
	O_Tx_G.get_xyz_angle_axis(O_r_G[0]);
	
		//jak cos przyjdzie glupiego z CEIH
	if (O_r_G[0][0]>100 || O_r_G[0][0]<-100)
	{
		for (int i=0; i<6; i++)
		{
			O_r_G[0][i]=O_r_G[1][i]; //EIH ONLY
		}
	}
	
	O_eps_EG_norm=0.0;
	for (int i=0; i<6; i++)
		{
			O_eps_EG[0][i]=(O_r_G[0][i]-O_r_E[0][i]);
			O_eps_EG_norm+=O_eps_EG[0][i]*O_eps_EG[0][i];
			O_eps_E[0][i]=gain[i]*O_eps_EG[0][i];		
		}
	
	for (int i=0; i<6; i++)
	{
		O_r_Ep[0][i]=O_r_E[0][i]+O_eps_E[0][i];
	}
	
	C_Tx_G.get_xyz_angle_axis(C_r_G[0]);
	
	if (node_counter==1)
	{
		for (int i=0; i<6; i++)
		{
			O_r_Ep[0][i]=O_r_E[0][i];
		}
	}
	
}


void ecp_vis_pb_eih_irp6ot::entertain_constraints(){
	// roznica w kroku -> docelowo predkosc
	for (int i=0; i<6; i++)
		O_r_Ep_d[0][i]=O_r_Ep[0][i]-O_r_E[0][i];

	// roznica w 2 krokach -> docelowo przyspieszenie
	for (int i=0; i<6; i++)
		O_r_Ep_d2[0][i]=O_r_Ep_d[0][i]-O_r_Ep_d[1][i];

	//ograniczenie przyspieszenia (opoznienie zalatwia regulator proporcjonalny)
	for (int i=0; i<6; i++)
	{
		if ((fabs(O_r_Ep_d2[0][i])>=d2_u_max[i]) && (fabs(O_r_Ep_d[0][i])
				>=fabs(O_r_Ep_d[1][i])))
		{
			if (O_r_Ep_d[0][i]>=0)
			{
				O_r_Ep_d[0][i]=O_r_Ep_d[1][i]+d2_u_max[i];
				O_r_Ep[0][i]=O_r_E[0][i]+O_r_Ep_d[0][i];
			}
			else if (O_r_Ep_d[0][i]<0)
			{
				O_r_Ep_d[0][i]=O_r_Ep_d[1][i]-d2_u_max[i];
				O_r_Ep[0][i]=O_r_E[0][i]+O_r_Ep_d[0][i];
			}
		}
	}
	
	O_Tx_Ep.set_xyz_angle_axis(O_r_Ep[0]);
	
	// ------------przepisanie wartosci-----
	for (int i=0; i<6; i++)
	{
		O_r_Ep[2][i]=O_r_Ep[1][i];
		O_r_Ep[1][i]=O_r_Ep[0][i];
		O_r_Ep_d[i][2]=O_r_Ep_d[i][1];
		O_r_Ep_d[i][1]=O_r_Ep_d[i][0];

		//O_r_G[1][i]=O_r_G[0][i];
		//C_r_G[1][i]=C_r_G[0][i];
	}

	
	O_Tx_Ep=O_Tx_Ep*G_Tx_G2;

	O_Tx_Ep.get_xyz_angle_axis(O_r_Ep[0]);
	
	std::cout << "ECP Ep: ";
	
	for (int i=0; i<6; i++)
	{
		the_robot->EDP_data.next_XYZ_AA_arm_coordinates[i] = O_r_Ep[0][i];
		std::cout << O_r_Ep[0][i] << " ";	
	}
	
	std::cout << std::endl;
	/*
	for (int i=0; i<6; i++)
	{
		the_robot->EDP_data.next_XYZ_AA_arm_coordinates[i] = the_robot->EDP_data.current_XYZ_AA_arm_coordinates[i];
	}
	*/

	
		the_robot->EDP_data.next_gripper_coordinate
			=the_robot->EDP_data.current_gripper_coordinate;
			
}


bool ecp_vis_pb_eih_irp6ot::first_step(void){

	vsp_vis_sac = sensor_m[SENSOR_CAMERA_SA];

	idle_step_counter = 1;
	vsp_vis_sac->base_period=0; //1
	vsp_vis_sac->current_period=0; //MAC7
	//vsp_force_irp6p->base_period=1;
	//td.interpolation_node_no = 1; //potrzebne? MAC7
	td.internode_step_no = 50; //step_no; 40
	td.value_in_step_no = td.internode_step_no - 5; //2 //10

	//TOOL


	the_robot->EDP_data.next_tool_frame[0][0]=1;
	the_robot->EDP_data.next_tool_frame[1][0]=0;
	the_robot->EDP_data.next_tool_frame[2][0]=0;
	the_robot->EDP_data.next_tool_frame[0][3]=0;

	the_robot->EDP_data.next_tool_frame[0][1]=0;
	the_robot->EDP_data.next_tool_frame[1][1]=1;
	the_robot->EDP_data.next_tool_frame[2][1]=0;
	the_robot->EDP_data.next_tool_frame[1][3]=0;

	the_robot->EDP_data.next_tool_frame[0][2]=0;
	the_robot->EDP_data.next_tool_frame[1][2]=0;
	the_robot->EDP_data.next_tool_frame[2][2]=1;
	the_robot->EDP_data.next_tool_frame[2][3]=0.25; 

	the_robot->EDP_data.instruction_type = SET_GET;
	the_robot->EDP_data.get_type = ARM_DV;
	the_robot->EDP_data.set_type = RMODEL_DV;
	the_robot->EDP_data.set_arm_type = XYZ_ANGLE_AXIS;
	the_robot->EDP_data.get_arm_type = FRAME;
	the_robot->EDP_data.set_rmodel_type = TOOL_FRAME;
	the_robot->EDP_data.get_rmodel_type = TOOL_FRAME;
	the_robot->EDP_data.next_interpolation_type= MIM; 
	the_robot->EDP_data.motion_type = ABSOLUTE; 

	the_robot->EDP_data.motion_steps = td.internode_step_no;
	the_robot->EDP_data.value_in_step_no = td.value_in_step_no;

	for (int i=0; i<6; i++)
	{
		the_robot->EDP_data.next_XYZ_AA_arm_coordinates[i] = 0;
		the_robot->EDP_data.next_force_xyz_torque_xyz[i] = 0;
	}
	
	O_eps_EG_norm=10;

	return true;
}
