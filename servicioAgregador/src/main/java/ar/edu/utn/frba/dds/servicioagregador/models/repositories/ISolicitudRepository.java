package ar.edu.utn.frba.dds.servicioagregador.models.repositories;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.EstadoSolicitud;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.SolicitudEliminacion;

import java.util.List;
import java.util.Optional;

public interface ISolicitudRepository {
  SolicitudEliminacion save(SolicitudEliminacion solicitud);
  List<SolicitudEliminacion> findAll();
  SolicitudEliminacion findById(Long id);
  List<EstadoSolicitud> findEstadosByIdSolicitud(Long idSolicitud);
  List<EstadoSolicitud> findEstadosPendientesByIdSolicitud(Long idSolicitud);
}

