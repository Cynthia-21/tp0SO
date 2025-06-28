package ar.edu.utn.frba.dds.servicioagregador.models.repositories.impl;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.EstadoSolicitud;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.PosibleEstado;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.solicitudes.SolicitudEliminacion;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.ISolicitudRepository;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

@Repository
public class SolicitudRepository implements ISolicitudRepository {
  private List<SolicitudEliminacion> solicitudes;

  public SolicitudRepository() {
    this.solicitudes = new ArrayList<>();
  }

  @Override
  public SolicitudEliminacion save(SolicitudEliminacion solicitud) {
    if (solicitud.getId() == null) {
      solicitud.setId(this.obtenerProximaId());
      this.solicitudes.add(solicitud);
    } else {
      this.solicitudes.remove(this.findById(solicitud.getId()));
      this.solicitudes.add(solicitud);
    }
    return solicitud;
  }

  @Override
  public List<SolicitudEliminacion> findAll() {
    return this.solicitudes;
  }

  @Override
  public SolicitudEliminacion findById(Long id) {
    return this.solicitudes.stream().filter(solicitud -> solicitud.getId().equals(id)).findFirst().orElse(null);
  }

  @Override
  public List<EstadoSolicitud> findEstadosByIdSolicitud(Long idSolicitud) {
    return this.findById(idSolicitud).getEstadosDeSolicitud();
  }

  @Override
  public List<EstadoSolicitud> findEstadosPendientesByIdSolicitud(Long idSolicitud) {
    return this.findById(idSolicitud)
        .getEstadosDeSolicitud()
        .stream()
        .filter(solicitud -> solicitud.getPosibleEstado().equals(PosibleEstado.PENDIENTE))
        .collect(Collectors.toList());
  }

  private Long obtenerProximaId() {
    return ((long) this.solicitudes.size() + 1);
  }
}
