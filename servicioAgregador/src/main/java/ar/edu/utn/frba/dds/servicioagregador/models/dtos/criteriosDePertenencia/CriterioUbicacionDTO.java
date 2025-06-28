package ar.edu.utn.frba.dds.servicioagregador.models.dtos.criteriosDePertenencia;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.Getter;
import lombok.NoArgsConstructor;

@Data
@Getter
@NoArgsConstructor
@AllArgsConstructor
public class CriterioUbicacionDTO extends CriterioPertenenciaDTO {
  private Double latitud;
  private Double longitud;
}
