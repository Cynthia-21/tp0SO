package ar.edu.utn.frba.dds.servicioagregador.services;

import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.EstadoSolicitudInputDto;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.SolicitudEliminacionInputDto;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.EstadoSolicitudOutputDto;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.SolicitudEliminacionOutputDto;

import java.util.List;

public interface ISolicitudService {
  SolicitudEliminacionOutputDto crearSolicitud(SolicitudEliminacionInputDto solicitud);
  EstadoSolicitudOutputDto cambiarEstadoSolicitud(Long solicitudId, EstadoSolicitudInputDto estadoSolicitud);
  SolicitudEliminacionOutputDto conseguirSolicitud(Long id);
  List<EstadoSolicitudOutputDto> obtenerEstadosDeSolicitudId(Long id);
}
